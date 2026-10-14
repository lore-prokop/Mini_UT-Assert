/*
**
** File: ut_hooks_stubs.h
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
**         CFS_EVS_stubs.h to make it a general purpose stub package for non
**         CFS systems. It was also combined with stubs.h to have one 
**         combined file for hooks and stub functionality. All cfs-related 
**         fuctions were removed. 
**         Lorraine Prokop / lore.prokop@yahoo.com
** 06/2026:  LEP Added #ifdef ENABLE_HOOKS_AND_STUBS to conditionally add
**           this capabaility for general purpose minimal utassert
**
** Purpose: Unit test header file example hooks and stubs used for testing.
**
*/
#ifdef ENABLE_HOOKS_AND_STUBS

#ifndef UT_HOOKS_STUBS_H
#define UT_HOOKS_STUBS_H

//#include "utassert.h"
//#include <common_types.h>

#define UT_MAX_INDEX    2000   // used for queing return codes in the unit test framework

/* List hook functions below. These are any functions requiring more complex behavior  */
/* than just default or custom stub return codes to perform unit testing.  These will  */
/* be called from within the stub function                                             */
int32 Ut_my_fcn_hook(int input);
                                         
typedef enum 
{
	/* define stub function list here */
    UT_STUB_1_INDEX,
	UT_MAX_STUB_INDEX
} Ut_FCN_INDEX_t;

typedef struct
{   
	int32 (*Ut_my_fcn_hook)(int input);
} Ut_HookTable_t;

typedef struct
{
    int32   Value;
    uint32  Count;
    boolean ContinueReturnCodeAfterCountZero;
} Ut_ReturnCodeTable_t;

/* Resets hook and return code tables for function hooks/stubs */
void Ut_Reset(void);

/* Assigns an override function to call, 'hook', when named function in hook table is called */
void Ut_SetFunctionHook(uint32 Index, void *FunPtr);

/* Assigns the desired 'stub' return code after call of index of named function */
void Ut_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt);

/* Instructs framework to continue after all user supplied return codes from */
/* function calls are used up, or not */
void Ut_ContinueReturnCodeAfterCountZero(uint32 Index);

/* To Customize -  test setup and teardown functions, if none needed use these*/
/* as default values for UtTest_Add() function                                */

void Test_Setup(void);
 
void Test_TearDown(void);

#endif
#endif
