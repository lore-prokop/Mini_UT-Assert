/*
**
** File: utassert.h (also includes uttest.h, utlist.h, uttools.h)
**
** $Id: utassert.h 1.1 2016/02/08 20:53:28EST rperera Exp  $
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
** $Log: utassert.h  $
** Revision 1.1 2016/02/08 20:53:28EST rperera 
** Initial revision
** Member added to project /FSW-TOOLs-REPOSITORY/ut-assert/inc/project.pj
** Revision 1.4 2015/06/16 16:02:14EDT sstrege 
** Added copyright information
** Revision 1.3 2015/03/17 18:13:15EDT czogby 
** --- Added comments ---  czogby [2015/03/31 18:01:28Z]
** Added macro UtAssert_IntegerCmpAbs
** Revision 1.2 2015/03/10 15:19:29EDT czogby 
** Add Missing Functionality to UT Assert Library

** Revision History:
** 2026-06   L.Prokop    Modified v1.1 obsolete version of the framework
**                       for non cfs and to be 1-file minimalistic.  
**                       Combined all framework files into one:  utassert,
**                       utlist, uttest, uttools, and utversion. Removed
**                       all cfs support.  Geared for small embedded
**                       projects. Reorganized and removed, but core code
**                       remains intact.
** 2026-10  L.Prokop     Found possibility of a divide by zero in the 
**                       UtAssert_DoubleCmpRel() macro, so fixed it...
**
*/

#ifndef _utassert_
#define	_utassert_

/*
 * Includes
 */

#include <common_types.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

/*--------------- UTASSERT Comparison Macros (utassert.h)---------------------*/
/** Design Notes:
**    - All asserts evaluate a expression as TRUE or FALSE to determine if a unit test has
**      passed or failed.  TRUE means the test passed, FALSE means the test failed.
**    - All asserts return a boolen result to indicate the pass fail status.
**    - All asserts are implemented as macros to hide the __LINE__ and __FILE__ macros.
**    - All asserts must call the function UtAssert.
*/
/*
 * Macro Definitions
 */

/* Evaluates a expression as either TRUE or FALSE.  TRUE means the test passed, FALSE means the test failed. */
#define     UtAssert_True(Expression, Description) \
                UtAssert(Expression, Description, __FILE__, __LINE__)

/* Evaluates a expression as either TRUE or FALSE.  TRUE means the test passed, FALSE means the test failed. */
#define     UtAssert_Bool(Expression, Description) \
                UtAssert(Expression, Description, __FILE__, __LINE__)

/* Asserts a test failure */
#define     UtAssert_Failed(Description) \
                UtAssert(FALSE, Description, __FILE__, __LINE__)

/* Compares two integers and determines if they are equal within a specified absolute tolerance. */
#define     UtAssert_IntegerCmpAbs(x, y, Tolerance, Description) \
                UtAssert((abs((x) - (y)) <= (Tolerance)), Description, __FILE__, __LINE__)

/* Compares two floating point numbers and determines if they are equal within a specified absolute tolerance. */
#define     UtAssert_DoubleCmpAbs(x, y, Tolerance, Description) \
                UtAssert((fabs((x) - (y)) <= (Tolerance)), Description, __FILE__, __LINE__)

/* Compares two floating point numbers and determines if they are equal within a specified relative tolerance. */
/* LEP FIX: Rewritten to use multiplication instead of division to eliminate the */
/* structural division-by-zero vulnerability when 'x' parameter is 0.0 */
#define     UtAssert_DoubleCmpRel(x, y, Ratio, Description) \
                UtAssert((fabs((x) - (y)) <= ((Ratio) * (x))), Description, __FILE__, __LINE__)
/* ORIG MACRO BODY - / (x): UtAssert((fabs((x) - (y))/(x) <= (Ratio)), Description, __FILE__, __LINE__)  */

/* Compares two strings and determines if they are equal. */
#define     UtAssert_StrCmp(String1, String2, Description) \
                UtAssert((strcmp(String1, String2) == 0), Description, __FILE__, __LINE__)

/* Compares at most Length characters of two strings and determines if they are equal. */
#define     UtAssert_StrnCmp(String1, String2, Length, Description) \
                UtAssert((strncmp(String1, String2, Length) == 0), Description, __FILE__, __LINE__)

/* Compares two regions of memory and determines if they are equal. */
#define     UtAssert_MemCmp(Memory1, Memory2, Length, Description) \
                UtAssert((memcmp(Memory1, Memory2, Length) == 0), Description, __FILE__, __LINE__)

/* Compares a region of memory to a static pattern and determines if they are equal.  Note: Use UtMemSet to
 * fill a region of memory with a static pattern. */
#define     UtAssert_MemCmpValue(Memory, Value, Length, Description) \
                UtAssert((UtMemCmpValue(Memory, Value, Length)), Description, __FILE__, __LINE__)

/* Compares a region of memory to a byte count pattern and determines if they are equal.  Note: Use UtMemFill to
 * fill a region of memory with a byte count pattern. */
#define     UtAssert_MemCmpCount(Memory, Length, Description) \
                UtAssert((UtMemCmpCount(Memory, Length)), Description, __FILE__, __LINE__)

/* Compares a region of memory with the contents of a binary file and determines if they are equal.  Note: Use
 * UtMem2BinFile to copy a region of memory to a binary file. */
#define     UtAssert_Mem2BinFileCmp(Memory, Filename, Description) \
                UtAssert((UtMem2BinFileCmp(Memory, Filename)), Description, __FILE__, __LINE__)

/*
 * Exported Functions
 */

/* Returns the number of asserts that have passed. */
uint32      UtAssert_GetPassCount(void);

/* Returns the number of asserts that have failed. */
uint32      UtAssert_GetFailCount(void);

/* Base assert function.  All asserts must call this function. */
boolean     UtAssert(boolean Expression, char *Description, char *File, uint32 Line);

/* Adds a new unit test to the test database. */
void    UtTest_Add(void (*Test)(void), void (*Setup)(void), void (*Teardown)(void), char *TestName);

/* Executes all unit tests contained in the test database.  Once all tests have finished executing 
 * a results summary is printed to the console and the test database is deleted.  This function also 
 * returns a boolean status indicating if any of the tests failed. (TRUE = at least one test failure 
 * has occurred, FALSE = all tests passed). This function is required in main to operate the framework */
int     UtTest_Run(void);


#endif


/* ----------------------------- utlist.h, linked list functionality (internal---------- */
/*
 * Macro Definitions
 */

/* Macros to redefine list functions to look like stack and queue functions */
#define UtStack_Push        UtList_Add
#define UtStack_Pop         UtList_RemoveLast
#define UtStack_IsEmpty     UtList_IsEmpty
#define UtStack_Depth       UtList_Depth

#define UtQueue_Add         UtList_Add
#define UtQueue_Get         UtList_RemoveFirst
#define UtQueue_Look        UtList_First
#define UtQueue_Delete      UtList_DeleteFirst
#define UtQueue_IsEmpty     UtList_IsEmpty
#define UtQueue_Depth       UtList_Depth

/*
 * Type Definitions
 */

typedef struct UtListNodeTag {
    struct UtListNodeTag   *Next;
    struct UtListNodeTag   *Prev;
    void                   *Data;
    uint32                  DataSize;
    uint32                  Tag;
} UtListNode_t;

 typedef struct {
    UtListNode_t           *First;
    UtListNode_t           *Last;
    uint32                  NumberOfEntries;
} UtListHead_t;

/*
 * Exported Functions
 */

/* Dynamically allocates a new list head.  A list head could also just be declared, this function is useful
 * if you need to dynamically allocate memory for a new list head.  Note always free list heads allocated by
 * this function by calling UtList_Destroy. */
UtListHead_t               *UtList_Create(void);

/* Frees a list head created by UtList_Create. */
void                        UtList_Destroy(UtListHead_t *ListHead);

/* Deletes all nodes on the list. */
void                        UtList_Reset(UtListHead_t *ListHead);

/* Dynamically adds a new node to the list.  Nodes are always added to the end of the list.  Memory is dynamically
 * allocated for the new node and to hold the data pointed to by Data.  A Tag field is also provided to be used to
 * store user defined information with the node. */
void                        UtList_Add(UtListHead_t *ListHead, void *Data, uint32 DataSize, uint32 Tag);

/* Deletes the first node from the list. */
void                        UtList_DeleteFirst(UtListHead_t *ListHead);

/* Deletes the last node from the list. */
void                        UtList_DeleteLast(UtListHead_t *ListHead);

/* Deletes the specified node from the list, this will screw up if you do not pass in a valid DeleteNode.  I do not
 * verify that DeleteNode is a member of the list. */
void                        UtList_DeleteNode(UtListHead_t *ListHead, UtListNode_t *DeleteNode);

/* Removes the first node from the list by first copying the data from the node to the memory buffer pointed to by the
 * specified Data pointer and then the node is deleted from the list.  Make sure the destination pointer points to a
 * memory buffer large enough to hold the data.  The size of the data on the node is available by referencing UtListNode->DataSize. */
void                        UtList_RemoveFirst(UtListHead_t *ListHead, void *Data);

/* Removes the last node from the list by first copying the data from the node to the memory buffer pointed to by the
 * specified Data pointer and then the node is deleted from the list.  Make sure the destination pointer points to a
 * memory buffer large enough to hold the data.  The size of the data on the node is available by referencing UtListNode->DataSize. */
void                        UtList_RemoveLast(UtListHead_t *ListHead, void *Data);

/* Removes the speciified RemoveNode from the list by first copying the data from the node to the memory buffer pointed to by the
 * specified Data pointer and then the node is deleted from the list.  Make sure the destination pointer points to a
 * memory buffer large enough to hold the data.  The size of the data on the node is available by referencing UtListNode->DataSize. */
void                        UtList_RemoveNode(UtListHead_t *ListHead, void *Data, UtListNode_t *RemoveNode);

/* Returns a pointer to the first node on the list.  This is the same as (UtListHead->First). */
UtListNode_t               *UtList_First(UtListHead_t *ListHead);

/* Returns a pointer to the last node on the list.  This is the same as (UtListHead->Last). */
UtListNode_t               *UtList_Last(UtListHead_t *ListHead);

/* Returns TRUE if the list is empty.  This is the same as (UtListHead->NumberOfEntries == 0). */
boolean                     UtList_IsEmpty(UtListHead_t *ListHead);

/* Returns the number of nodes on the list.  This is the same as (UtListHead->NumberOfEntries). */
uint32                      UtList_Depth(UtListHead_t *ListHead);

/* ----------------------------- uttools.h, helper functions ------------------- */

#define             UtMemSet memset

/* Copies a region of memory to a binary file.  This file can be reloaded by calling UtBinFile2Mem or it can be
 * used to verify test results by calling UtMem2BinFileCmp. */
boolean             UtMem2BinFile(void *Memory, char *Filename, uint32 Length);

/* Copies a binary file to a region of memory. */
boolean             UtBinFile2Mem(void *Memory, char *Filename, uint32 Length);

/* Copies a region of memory to a hex file */
boolean             UtMem2HexFile(void *Memory, char *Filename, uint32 Length);

/* Fills a region of memory with a byte count pattern. */
void                UtMemFill(void *Memory, uint32 Length);

/* Just like the standard printf except it will supress its output unless the macro UT_VERBOSE
 * is defined. */
void                UtPrintf(char *Spec, ...);

/* Just like the standard sprintf except it returns a pointer to the result string.  The result string
 * cannot be larger than 256 bytes.  */
char               *UtSprintf(char *Spec, ...);

/* Calls UtPrintf to print a range of memory as hex bytes. */
void                UtPrintx(void *Memory, uint32 Length);

/* Compares a region of memory to a static pattern and determines if they are equal.  Note: Use UtMemSet to
 * fill a region of memory with a static pattern. */
boolean             UtMemCmpValue(void *Memory, uint8 Value, uint32 Length);

/* Compares a region of memory to a byte count pattern and determines if they are equal.  Note: Use UtMemFill to
 * fill a region of memory with a byte count pattern. */
boolean             UtMemCmpCount(void *Memory, uint32 Length);

/* Compares a region of memory with the contents of a binary file and determines if they are equal.  Note: Use
 * UtMem2BinFile to copy a region of memory to a binary file. */
boolean             UtMem2BinFileCmp(void *Memory, char *Filename);