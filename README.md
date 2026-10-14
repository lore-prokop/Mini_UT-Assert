#  Mini_UT-Assert

--------------------------------------------------------------------------------
Mini UT-Assert Unit Test Framework
--------------------------------------------------------------------------------
Version: 1.0  
Date: 2026-10-14

--------------------------------------------------------------------------------
1. OVERVIEW
--------------------------------------------------------------------------------
This repo provides a miniature version of NASA's UT-Assert unit test framework.
It supports Boolean, integer, float, memory and file compare assertions;
hook and stub functionality, and console output through running multiple tests 
from a single main(). This version added no capability to the NASA framework, 
only removed hooks and stubs for the cFS framework, reduced the number of files 
to 2 by consolidating code, and provided customizable stub and hook functions.
The framework contains only 286 logical source lines of code (SLOC).  This unit 
test framework is intended for use in small C-language embedded systems, and 
may be accompanied by another repo containing a requirements and certification 
test suite for Mini UT-Assert if tool qualification is needed for safety-
critical applications.

	Optional: Mini UT-Assert_Cert_tests - An accompanying requirements set and
	  test suite if tool certification is required by the project can be found 
	  at: https://github.com/lore-prokop/Mini_UT-Assert_Cert_Tests
	  
	Note: the original NASA UT-Assert may be found here:
	  https://sourceforge.net/projects/cfs-ut-assert/files/.  

--------------------------------------------------------------------------------
2. FEATURES
--------------------------------------------------------------------------------
- General-purpose unit test framework supporting Boolean, integer, float, memory
  and file compare assertions, hook and stub functionality, and console output
- Multiple unit tests can be run under a single main()
- Supports C (c17, or equivalently iso9899:2017) language tests
- Console Output of "pass" or "fail"  and summary
- This software is released under the NASA Open Source Agreement (NOSA) license
--------------------------------------------------------------------------------
3. SYSTEM REQUIREMENTS
--------------------------------------------------------------------------------
- gcc or equivalent C compiler

--------------------------------------------------------------------------------
4. INSTALLATION and USAGE
--------------------------------------------------------------------------------
The overall usage is as follows: 

- Download the Mini UT-Assert Package  
- Create Unit tests by calling "UtTest_Add", use assert functions within them:
	UtTest_Add(fcn, Test_Setup, Test_TearDown, "Test Name");
	UtAssert_True (some_test,"String to print to console");

- Run tests by calling UtTest_Run() within main()
	- Compile and link your main test runner program with utassert.c/h and 
	  ut_hooks_stubs.c/h
	- Note: link to your own stub functions if override is desired, can use your 
	  variables and data by "externing" them and including your own header files
	  within your main test runner file
	
- Customize behavior (if needed) by adding the following and calling API 
  in ut_hooks_stubs.c:
	- custom stub return values
	- custom hook functions
	- custom test setup and teardown functions

--------------------------------------------------------------------------------
5. FILE and DIRECTORY DESCRIPTIONS
--------------------------------------------------------------------------------
Primary files for this repo:
* utassert.c & .h  (in /src and /inc directories, respectively)
* ut_hooks_stubs.c & .h
* /doc directory contains NASA requirements, tutorial, and other documentation
       that came with original NASA distro

--------------------------------------------------------------------------------
6. KNOWN FEATURES / CONSTRAINTS / USAGE GUIDELINES / NOTES
--------------------------------------------------------------------------------
* C Language Version -
The version of the C language standard chosen is C17, also known as 
iso9899:2017.  One of the requirements in the test suite verifies this.
 
--------------------------------------------------------------------------------
7. CONTRIBUTING
--------------------------------------------------------------------------------
Contributions are welcome to expand or fix this. Please contact author.

--------------------------------------------------------------------------------
8. LICENSE
--------------------------------------------------------------------------------
NASA Open Source Agreement (NOSA) License

Please see the file license-ut-assert_nosa.pdf in this repo for more details.

--------------------------------------------------------------------------------
9. CONTACT
--------------------------------------------------------------------------------
Author:  Lorraine E. Prokop
Email: lore.prokop@gmail.com
