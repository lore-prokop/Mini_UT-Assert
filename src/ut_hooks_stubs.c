/*
** File:  ut_hooks_stubs.c, based on ut_cfe_evs_stubs.c
**
** $Id: ut_cfe_evs_stubs.c 1.2 2016/07/08 17:22:25EDT czogby Exp  $
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
** 9/2025: This file has been modified extensively to adapt the pattern from 
**         CFS_EVS_stubs.h to make it a general purpose stub and hook package 
**         for non CFS systems.  All CFS functions were removed. 
*          Author:  Lorraine Prokop / lore.prokop@gmail.com. It was also
**         combined with hooks files and functions.  
**
** Purpose: General Purpose Unit test framework for implementing function
**          return code stubs, and to provide the ability to call additional
**          "hook" functions if more extenstive behavior than return codes
**          is desired.  Also provides default (void) test setup and teardown
**          functions to customize and copy for each UtTest_Add()
** 		
** Usage:   The user should customize and add to the functions in this file.
**          Add custom stub return values, hook fuctions, and test setup and 
**          teardown functions as needed.  Use this as an example.
*/

/*
** Include section
*/

#ifdef ENABLE_HOOKS_AND_STUBS   /* compile in and customize as needed */

//#include <fsw.h>   // the real fsw headers should be included here, but in the makefile the alternate .o is linked in
#include <common_types.h>  // replace this with your systems common type definitions here
#include "utassert.h"
#include "ut_hooks_stubs.h"
#include <string.h>

Ut_HookTable_t          Ut_HookTable;
Ut_ReturnCodeTable_t    Ut_ReturnCodeTable[UT_MAX_INDEX];

/* -------------- hooking & stubbing support fcns ----------------------------*/

void Ut_Reset(void)
{
    memset(&Ut_HookTable, 0, sizeof(Ut_HookTable));
    memset(&Ut_ReturnCodeTable, 0, sizeof(Ut_ReturnCodeTable));
    
}

void Ut_SetFunctionHook(uint32 Index, void *FunPtr)
{
    if   (Index == UT_STUB_1_INDEX)    
	{ 
		Ut_HookTable.Ut_my_fcn_hook = FunPtr; 
	}
    else
    {
        UtPrintf("Unsupported Index In Ut_SetFunctionHook Call %lu", (long unsigned int)Index);
        UtAssert_True(FALSE, "Unsupported Hook Index In SetFunctionHook Call");
    }
}

void Ut_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt)
{
    if (Index < UT_MAX_INDEX)
    {
        Ut_ReturnCodeTable[Index].Value = RtnVal;
        Ut_ReturnCodeTable[Index].Count = CallCnt;
    }
    else
    {
        UtPrintf("Unsupported Index In SetReturnCode Call %lu\n", (long unsigned int)Index);
        UtAssert_True(FALSE, "Unsupported Index In SetReturnCode Call");
    }
}

boolean Ut_UseReturnCode(uint32 Index)
{
    if (Ut_ReturnCodeTable[Index].Count > 0)
    {
        Ut_ReturnCodeTable[Index].Count--;
        if (Ut_ReturnCodeTable[Index].Count == 0)
            return(TRUE);
    }
    else if (Ut_ReturnCodeTable[Index].ContinueReturnCodeAfterCountZero == TRUE)
    {
        return(TRUE);
    }
    
    return(FALSE);
}

void Ut_ContinueReturnCodeAfterCountZero(uint32 Index)
{
    Ut_ReturnCodeTable[Index].ContinueReturnCodeAfterCountZero = TRUE;
}

/* --------- BEGIN CUSTOMIZEABLE SECTION ------------------------------------ */

/* --------- Function Return Code Stubbing Here --  Customize   ------------- */
/* Reimplement functions in the real software here, link in the makefile.     */
/* This will override their return behavior during unit testing.              */
/* If only a return value change is needed, use only this stub, if more       */
/* complex behavior is needed, use another function, a function "hook" to     */
/* provide the desired behavior and return code.   If no return code or       */
/* function hook is provided, return a default value, in this case the value  */
/* echos input by default                                                */

int32 my_fcn(int input)
{
    /* Check for specified return code, if one there, return that */
	if (Ut_UseReturnCode(UT_STUB_1_INDEX))
	{
		return Ut_ReturnCodeTable[UT_STUB_1_INDEX].Value;
	}
	
	/* Check for Function Hook, use that function if it exists */
    if (Ut_HookTable.Ut_my_fcn_hook)
	{
        return(Ut_HookTable.Ut_my_fcn_hook(input));	
	}
	
	return input;  	/* echo input by default, can be useful, or change to return true */
}

/* ----------Function Hooks --- To Customize   -------------------------------*/
/* Add function hooks source code here if special behavior of functions is    */
/* needed that is different than just returning a specified return value      */

int Ut_my_fcn_hook(int input)
{
    return (input*2);  //different behavior than default return codes above
}

#endif

/* --------- Default Placeholder Functions for Test Setup and Tear Down ------*/
/*           Put this function, or a custom one, in each UtTest_Add() call    */
/* ---------------- Customize or move this as needed ------------------------ */
 
void Test_Setup(void)
{
    /* initialize test environment to default state for each test */

}   /* end Example_Test_Setup */
 
 void Test_TearDown(void)
{
    /* cleanup test environment */
	
}   /* end Example_Test_TearDown */
