/*
**
** File: utassert.c (and uttools.c, utlist.c, uttest.c)
**
** $Id: utassert.c 1.1 2016/02/08 20:53:31EST rperera Exp  $
**
** Copyright 2012-2013 United States Government as represented by the 
** Administrator of the National Aeronautics and Space Administration. 
** All Other Rights Reserved.  
**
** This software was created at NASA's Goddard Space Flight Center.
** This software is governed by the NASA Open Source Agreement and may be 
** used, distributed and modified only pursuant to the terms of that 
** agreement.
**
** Purpose: This code implements a standard set of asserts for use in unit tests.
**
** $Log: utassert.c  $
** Revision 1.1 2016/02/08 20:53:31EST rperera 
** Initial revision
** Member added to project /FSW-TOOLs-REPOSITORY/ut-assert/src/project.pj
** Revision 1.3 2015/06/16 16:14:04EDT sstrege 
** Added copyright information
** Revision 1.2 2015/03/10 15:21:43EDT czogby 
** Add Missing Functionality to UT Assert Library
** --- Added comments ---  czogby [2015/03/31 18:36:49Z]
** 2026-06   L.Prokop    Modified this v1.1 obsolete version of the framework
**                       for non cfs and to be 1-file minimalistic.  
**                       Combined all framework files into one:  utassert,
**                       utlist, uttest, uttools, and utversion. Removed
**                       all cfs support.
*/

/*
 * Includes
 */

#include "common_types.h"
#include "utassert.h"
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>


/*-------------------- utassert.c ---------------------------------------------*/
/*
 * Local Data
 */

uint32      UtAssertPassCount = 0;
uint32      UtAssertFailCount = 0;

/*
 * Function Definitions
 */

uint32 UtAssert_GetPassCount(void)
{
    return(UtAssertPassCount);
}

uint32 UtAssert_GetFailCount(void)
{
    return(UtAssertFailCount);
}

boolean UtAssert(boolean Expression, char *Description, char *File, uint32 Line)
{
    if (Expression) {
        #ifdef UT_VERBOSE
        printf("PASS: %s\n", Description);
        #endif
        UtAssertPassCount++;
        return(TRUE);
    }
    else {
        printf("FAIL: %s, File: %s, Line: %lu\n", Description, File, (long unsigned int )Line);
        UtAssertFailCount++;
        #ifdef UT_STOP_ON_ASSERT_FAILURE
        printf("********** Unit Test Aborted **********\n");
        exit(1);
        #endif
        return(FALSE);
    }
}

/*-------------------- uttest.c ----------------------------------------------*/

typedef struct {
    void    (*Test)(void);
    void    (*Setup)(void);
    void    (*Teardown)(void);
    char     *TestName;
} UtTestDataBaseEntry_t;

/*
 * Local Data
 */

UtListHead_t    UtTestDataBase;
uint32          UtTestsExecutedCount = 0;

/*
 * Function Definitions
 */

void UtTest_Add(void (*Test)(void), void (*Setup)(void), void (*Teardown)(void), char *TestName)
{
    UtTestDataBaseEntry_t   UtTestDataBaseEntry;

    UtTestDataBaseEntry.Test = Test;
    UtTestDataBaseEntry.Setup = Setup;
    UtTestDataBaseEntry.Teardown = Teardown;
    UtTestDataBaseEntry.TestName = TestName;
    UtList_Add(&UtTestDataBase, &UtTestDataBaseEntry, sizeof(UtTestDataBaseEntry_t), 0);
}

int UtTest_Run(void)
{
    uint32                   i;
    UtListNode_t            *UtListNode;
    UtTestDataBaseEntry_t   *UtTestDataBaseEntry;
    
    if (UtTestDataBase.NumberOfEntries > 0) {
        
        UtListNode = UtTestDataBase.First;
        for (i=0; i < UtTestDataBase.NumberOfEntries; i++) {
            
            UtTestDataBaseEntry = UtListNode->Data;

            #if defined UT_VERBOSE_TEST_NAME     /* || defined UT_VERBOSE, L. Prokop removed so only see "pass-fail" */
            if (strlen(UtTestDataBaseEntry->TestName) > 0) { printf("\nRunning Test: %s\n", UtTestDataBaseEntry->TestName); }
            #endif

            if (UtTestDataBaseEntry->Setup)    { UtTestDataBaseEntry->Setup(); }
            if (UtTestDataBaseEntry->Test)     { UtTestDataBaseEntry->Test(); UtTestsExecutedCount++; }
            if (UtTestDataBaseEntry->Teardown) { UtTestDataBaseEntry->Teardown(); }

            UtListNode = UtListNode->Next;
        }
    }

    printf("\n");
    printf("Tests Executed:    %lu\n", (long unsigned int) UtTestsExecutedCount);
    printf("Assert Pass Count: %lu\n", (long unsigned int) UtAssert_GetPassCount());
    printf("Assert Fail Count: %lu\n", (long unsigned int) UtAssert_GetFailCount());
 
    UtList_Reset(&UtTestDataBase);

    return (UtAssert_GetFailCount() > 0);
}

/*-------------------- utlist.c ----------------------------------------------*/
/*
 * Function Definitions
 */

UtListHead_t *UtList_Create(void)
{
    UtListHead_t *NewList;

    NewList = malloc(sizeof(UtListHead_t));
    NewList->First = NULL;
    NewList->Last = NULL;
    NewList->NumberOfEntries = 0;
    return (NewList);
}

void UtList_Destroy(UtListHead_t *ListHead)
{
    UtList_Reset(ListHead);
    free(ListHead);
}

void UtList_Reset(UtListHead_t *ListHead)
{
    while (!UtList_IsEmpty(ListHead)) {
        UtList_DeleteFirst(ListHead);
    }
}

void UtList_Add(UtListHead_t *ListHead, void *Data, uint32 DataSize, uint32 Tag)
{
    UtListNode_t *NewNode = NULL;
    
    NewNode = malloc(sizeof(UtListNode_t));
    if (ListHead->NumberOfEntries == 0) {

        ListHead->First = NewNode;
        ListHead->Last = NewNode;
        ListHead->NumberOfEntries++;

        NewNode->Next = NULL;
        NewNode->Prev = NULL;
        NewNode->Tag = Tag;
        NewNode->DataSize = DataSize;
        NewNode->Data = malloc(DataSize);
        memcpy(NewNode->Data, Data, DataSize);
    }
    else {

        NewNode->Next = NULL;
        NewNode->Prev = ListHead->Last;
        NewNode->Tag = Tag;
        NewNode->DataSize = DataSize;
        NewNode->Data = malloc(DataSize);
        memcpy(NewNode->Data, Data, DataSize);

        ListHead->Last->Next = NewNode;
        ListHead->Last = NewNode;
        ListHead->NumberOfEntries++;
    }
}

void UtList_DeleteFirst(UtListHead_t *ListHead)
{
    UtList_DeleteNode(ListHead, ListHead->First);
}

void UtList_DeleteLast(UtListHead_t *ListHead)
{
    UtList_DeleteNode(ListHead, ListHead->Last);
}

void UtList_DeleteNode(UtListHead_t *ListHead, UtListNode_t *DeleteNode)
{
   
    if (!UtList_IsEmpty(ListHead)) {

        if (ListHead->NumberOfEntries == 1) {
            ListHead->First = NULL;
            ListHead->Last = NULL;
            ListHead->NumberOfEntries = 0;
        }
        else if (DeleteNode == ListHead->First) {
            ListHead->First = DeleteNode->Next;
            ListHead->First->Prev = NULL;
            ListHead->NumberOfEntries--;
        }
        else if (DeleteNode == ListHead->Last) {
            ListHead->Last = DeleteNode->Prev;
            ListHead->Last->Next = NULL;
            ListHead->NumberOfEntries--;        
        }
        else {
            DeleteNode->Prev->Next = DeleteNode->Next;
            DeleteNode->Next->Prev = DeleteNode->Prev;
            ListHead->NumberOfEntries--;
        }

        free(DeleteNode->Data);
        free(DeleteNode);
    }
}

void UtList_RemoveFirst(UtListHead_t *ListHead, void *Data)
{
    UtList_RemoveNode(ListHead, Data, ListHead->First);
}

void UtList_RemoveLast(UtListHead_t *ListHead, void *Data)
{
    UtList_RemoveNode(ListHead, Data, ListHead->Last);
}

void UtList_RemoveNode(UtListHead_t *ListHead, void *Data, UtListNode_t *CurrentNode)
{
    if (!UtList_IsEmpty(ListHead)) {
        memcpy(Data, CurrentNode->Data, CurrentNode->DataSize);
        UtList_DeleteNode(ListHead, CurrentNode);
    }
}

UtListNode_t *UtList_First(UtListHead_t *ListHead)
{
    return(ListHead->First);
}

UtListNode_t *UtList_Last(UtListHead_t *ListHead)
{
    return(ListHead->Last);
}

boolean UtList_IsEmpty(UtListHead_t *ListHead)
{
    return(ListHead->NumberOfEntries == 0);
}

uint32 UtList_Depth(UtListHead_t *ListHead)
{
    return(ListHead->NumberOfEntries);
}

/*-------------------- uttools.c----------------------------------------------*/

/*
 * Function Definitions
 */

boolean UtMem2BinFile(void *Memory, char *Filename, uint32 Length)
{
    FILE   *fp;

    if ((fp = fopen(Filename, "w"))) {
        fwrite(Memory, Length, 1, fp);
        fclose(fp);
        return(TRUE);
    }
    else
    {
        printf("UtMem2BinFile: Error Opening File: %s, %s\n", Filename, strerror(errno));
        UtAssert_True(FALSE, "UtMem2BinFile: Error Opening File");
        return(FALSE);
    }
}

boolean UtBinFile2Mem(void *Memory, char *Filename, uint32 Length)
{
    FILE   *fp;

    if ((fp = fopen(Filename, "r"))) {     
        fread(Memory, Length, 1, fp);
        fclose(fp);
        return(TRUE);
    }
    else
    {
        printf("UtBinFile2Mem: Error Opening File: %s, %s\n", Filename, strerror(errno));
        UtAssert_True(FALSE, "UtBinFile2Mem: Error Opening File");
        return(FALSE);
    }
}

boolean UtMem2HexFile(void *Memory, char *Filename, uint32 Length)
{
    FILE        *fp;
    uint32       i;
    uint32       j;

    if ((fp = fopen(Filename, "w"))) {

        for (i=0; i < Length; i+=16) {
            fprintf(fp, "   %06lX: ", (long unsigned int)i);
            for (j=0; j < 16; j++) {
                if ((i+j) < Length)
                    fprintf(fp, "%02X ", ((uint8 *)Memory)[i+j]);
                else
                    fprintf(fp, "   ");
            }
            fprintf(fp, " ");
            for (j=0; j < 16; j++) {
                if ((i+j) < Length)
                    fprintf(fp, "%c", isprint(((uint8 *)Memory)[i+j]) ? ((uint8 *)Memory)[i+j] : '.');
            }
            fprintf(fp, "\n");
        }
        fclose(fp);
        return(TRUE);
    }
    else {
        printf("UtMem2HexFile: Error Opening File: %s, %s\n", Filename, strerror(errno));
        UtAssert_True(FALSE, "UtMem2HexFile: Error Opening File");
        return(FALSE);
    }
}

void UtMemFill(void *Memory, uint32 Length)
{
    uint32 i;
    uint8  *Byte_ptr = Memory;

    for(i=0; i < Length; i++) {
        Byte_ptr[i] = i;
    }
}

void UtPrintf(char *Spec, ...)
{
#ifdef UT_VERBOSE
    va_list         Args;
    static char     Text[256];

    va_start(Args, Spec);
    vsprintf(Text, Spec, Args);
    va_end(Args);

    printf("%s", Text);
#else
    (void)Spec;
#endif
}

char *UtSprintf(char *Spec, ...)
{
    va_list         Args;
    static char     Text[10][256];
    static uint32   TextIndex = 0;

    if (TextIndex >= 10) TextIndex = 0;

    va_start(Args, Spec);
    vsprintf(Text[TextIndex], Spec, Args);
    va_end(Args);

    return(Text[TextIndex++]);
}

void UtPrintx(void *Memory, uint32 Length)
{
    uint32 i;
    uint8  *Byte_ptr = Memory;

    for (i=0; i < Length; i++) {
        UtPrintf("%02X ", Byte_ptr[i]);
    }
    UtPrintf("\n");
}

boolean UtMemCmpValue(void *Memory, uint8 Value, uint32 Length)
{
    uint32 i;
    uint8  *Byte_ptr = Memory;

    for (i=0; i < Length; i++) {
        if (Byte_ptr[i] != Value) {
            return(FALSE);
        }
    }
    return (TRUE);
}

boolean UtMemCmpCount(void *Memory, uint32 Length)
{
    uint32 i;
    uint8  *Byte_ptr = Memory;

    for (i=0; i < Length; i++) {
        if (Byte_ptr[i] != (i & 0xFF)) {
           return(FALSE);
        }
    }
    return (TRUE);
}

boolean UtMem2BinFileCmp(void *Memory, char *Filename)
{
    FILE   *fp;
    uint8  *MemByte = Memory;
    int     FileByte;
    boolean Success;
    uint32  i;

    Success = TRUE;
    if ((fp = fopen(Filename, "r"))) {

        for (i=0; (FileByte = fgetc(fp)) != EOF; i++) {
            if (MemByte[i] != FileByte) {
                Success = FALSE;
                printf("UtMem2BinFileCmp: Miscompare in file: %s, byte offset: %lu, expected: %u, found: %u\n", Filename, (long unsigned int)i, MemByte[i], FileByte);
                break;
            }            
        }
        fclose(fp);
    }
    else {
        Success = FALSE;
        printf("UtMem2BinFileCmp: Error Opening File: %s, %s\n", Filename, strerror(errno));
        UtAssert_True(FALSE, "UtMem2BinFileCmp: Error Opening File");
    }

    return(Success);
}
