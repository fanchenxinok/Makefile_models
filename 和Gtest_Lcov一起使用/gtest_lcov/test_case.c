// Copyright 2005, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// A sample program demonstrating using Google C++ testing framework.
//
// Author: wan@google.com (Zhanyong Wan)


// This sample shows how to write a simple unit test for a function,
// using Google C++ testing framework.
//
// Writing a unit test using Google C++ testing framework is easy as 1-2-3:


// Step 1. Include necessary header files such that the stuff your
// test logic needs is declared.
//
// Don't forget gtest.h, which declares the testing framework.

#include <limits.h>
#include "gtest/gtest.h"

#include "myComDef.h"
#include "responser12.h"
#include "listener12.h"

extern "C" {
	#include "log_out.h"
}



// Step 2. Use the TEST macro to define your tests.
//
// TEST has two parameters: the test case name and the test name.
// After using the macro, you should define your test logic between a
// pair of braces.  You can use a bunch of macros to indicate the
// success or failure of a test.  EXPECT_TRUE and EXPECT_EQ are
// examples of such macros.  For a complete list, see gtest.h.
//
// <TechnicalDetails>
//
// In Google Test, tests are grouped into test cases.  This is how we
// keep test code organized.  You should put logically related tests
// into the same test case.
//
// The test case name and the test name should both be valid C++
// identifiers.  And you should not use underscore (_) in the names.
//
// Google Test guarantees that each test you define is run exactly
// once, but it makes no guarantee on the order the tests are
// executed.  Therefore, you should write your tests in such a way
// that their results don't depend on their order.
//
// </TechnicalDetails>


// Tests Factorial().

// Tests factorial of negative numbers.

static responser1 res1_instance;
static responser2 res2_instance;

static listener1 lis1_instance;
static listener2 lis2_instance;


TEST(apl_registResponserTest, isTrue)
{
	ASSERT_EQ(TRUE, apl_registResponser(RES1, RES_MID, &res1_instance));
       ASSERT_EQ(TRUE, apl_registResponser(RES2, RES_HIGH, &res2_instance));
}


TEST(apl_registListenerTest, isTrue)
{
    ASSERT_EQ(TRUE, apl_registListener(RES1, &res1_instance, &lis1_instance));
    ASSERT_EQ(TRUE, apl_registListener(RES2, &res2_instance, &lis2_instance));
}


TEST(loop_send_test, isRight)
{
    int n = 0;
    while(n < 10){
		 MSG_INFO msgInfo;
		 char msgData[128];

		 msgInfo.resType = RES1;  //设置动作对象
		 msgInfo.msg.eventID = R1_FUN1; //设置应该做什么动作
		 EXPECT_EQ(TRUE, apl_sendMessage(RES1, R1_FUN1, msgData)); //发送开始消息及动作参数

		 //apl_unRegistResponser(RES1);

		 msgInfo.msg.eventID = R1_FUN2;
		 int a = 10, b = 122;
		 apl_msgPacker(msgData,sizeof(int), &a,sizeof(int), &b, -1);
		 EXPECT_EQ(TRUE, apl_sendMessage(RES1, R1_FUN2, msgData));

		 msgInfo.resType = RES2;
		 msgInfo.msg.eventID = R2_FUN1;
		 double c = 4.23, d = 2.32;
		 apl_msgPacker(msgData,sizeof(double), &c,sizeof(double), &d, -1);
		 EXPECT_EQ(TRUE, apl_sendMessage(RES2, R2_FUN1, msgData));

		 msgInfo.msg.eventID = R2_FUN2;
		 a = 20;
		 b = 30;
		 apl_msgPacker(msgData,sizeof(int), &a,sizeof(int), &b, -1);
		 EXPECT_EQ(TRUE, apl_sendMessage(RES2, R2_FUN2, msgData));

		 sleep(1);
               ++n;
 	}
}


// Step 3. Call RUN_ALL_TESTS() in main().
//
// We do this by linking in src/gtest_main.cc file, which consists of
// a main() function which calls RUN_ALL_TESTS() for us.
//
// This runs all the tests you've defined, prints the result, and
// returns 0 if successful, or 1 otherwise.
//
// Did you notice that we didn't register the tests?  The
// RUN_ALL_TESTS() macro magically knows about all the tests we
// defined.  Isn't this convenient?
