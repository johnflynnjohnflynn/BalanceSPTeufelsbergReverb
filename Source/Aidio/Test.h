//--------//--------//--------//--------//--------//--------//--------//--------
/*
    The MIT License (MIT)

    Copyright (c) 2016 John Flynn

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to
    deal in the Software without restriction, including without limitation the
    rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
    sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.
*/
//--------//--------//--------//--------//--------//--------//--------//--------

#ifndef TEST_H_INCLUDED
#define TEST_H_INCLUDED

//--------//--------//--------//--------//--------//--------//--------//--------
/** 
    Special version of jassert that throws exception instead of assertion when
    unit tests are switched on with the JF_UNIT_TESTS=1 preprocessor flag.
    
    Note: this won't break at the proper point during debugging. If your hitting
    an assert unrelated to the unit tests, try set JF_UNIT_TESTS=0
    
    This will trigger warning:
    "Lexical or Preprocessor Issue 'jassertfalse' macro redefined"

    @see jassert
*/

struct JassertError { JassertError() {} };

#if AIDIO_UNIT_TESTS
 #undef jassertfalse
 #define jassertfalse JUCE_BLOCK_WITH_FORCED_SEMICOLON (throw JassertError();)
#endif

//--------//--------//--------//--------//--------//--------//--------//--------
/** 
    Boilerplate macro to make sure a static instance of a test gets created in 
    the header. (Otherwise an instance of the class will need to be created 
    somewhere else before the tests run.)
    
    @example (...look in test/main.cpp...)

    @see UnitTestRunner
*/

#if AIDIO_UNIT_TESTS
 #define AIDIO_DECLARE_UNIT_TEST_WITH_STATIC_INSTANCE(TestClassName)        \
 class TestClassName  : public UnitTest                                     \
 {                                                                          \
 public:                                                                    \
     TestClassName();                                                       \
     void runTest() override;                                               \
 };                                                                         \
 static TestClassName TestClassName; // eek! instance same as class name
#else
 #define AIDIO_DECLARE_UNIT_TEST_WITH_STATIC_INSTANCE(TestClassName) // do nothing
#endif

#endif  // TEST_H_INCLUDED
