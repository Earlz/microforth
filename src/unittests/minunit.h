//taken from https://github.com/siu/minunit

/* 
Copyright (c) 2012 David Siñuela Pastor, siu.4coders@gmail.com

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#ifndef __MINUNIT_H__
#define __MINUNIT_H__

#ifdef __cplusplus
        extern "C" {
#endif

#include <stdlib.h>


#if defined(_WIN32)
#include <Windows.h>

#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))


/* Change POSIX C SOURCE version for pure c99 compilers */
#define _POSIX_C_SOURCE 200112L

#include <unistd.h>     /* POSIX flags */
#include <time.h>       /* clock_gettime(), time() */
#include <sys/time.h>   /* gethrtime(), gettimeofday() */
#include <sys/resource.h>
#include <sys/times.h>

#if defined(__MACH__) && defined(__APPLE__)
#include <mach/mach.h>
#include <mach/mach_time.h>
#endif

#else
#error "Unable to define timers for an unknown OS."
#endif

#include <stdio.h>
#include <math.h>

// Maximum length of last message
#define MINUNIT_MESSAGE_LEN 1024
// Do not change
#define MINUNIT_EPSILON 1E-12

// Misc. counters
static int minunit_run = 0;
static int minunit_assert = 0;
static int minunit_fail = 0;
static int minunit_status = 0;

// Timers
static double minunit_real_timer = 0;
static double minunit_proc_timer = 0;

// Last message
static char minunit_last_message[MINUNIT_MESSAGE_LEN];

// Test setup and teardown function pointers
static void (*minunit_setup)(void) = NULL;
static void (*minunit_teardown)(void) = NULL;

// Definitions
#define MU_TEST(method_name) static void method_name()
#define MU_TEST_SUITE(suite_name) static void suite_name()

#define MU__SAFE_BLOCK(block) do {\
        block\
} while(0)

// Run test suite and unset setup and teardown functions
#define MU_RUN_SUITE(suite_name) MU__SAFE_BLOCK(\
        suite_name();\
        minunit_setup = NULL;\
        minunit_teardown = NULL;\
)

// Configure setup and teardown functions
#define MU_SUITE_CONFIGURE(setup_fun, teardown_fun) MU__SAFE_BLOCK(\
        minunit_setup = setup_fun;\
        minunit_teardown = teardown_fun;\
)

// Test runner
#define MU_RUN_TEST(test) MU__SAFE_BLOCK(\
        if (minunit_real_timer==0 && minunit_real_timer==0) {\
                minunit_real_timer = mu_timer_real();\
                minunit_proc_timer = mu_timer_cpu();\
        }\
        if (minunit_setup) (*minunit_setup)();\
        minunit_status = 0;\
        test();\
        minunit_run++;\
        if (minunit_status) {\
                minunit_fail++;\
                printf("F");\
                printf("\n%s\n", minunit_last_message);\
        }\
        fflush(stdout);\
        if (minunit_teardown) (*minunit_teardown)();\
)

// Report
#define MU_REPORT() MU__SAFE_BLOCK(\
        printf("\n\n%d tests, %d assertions, %d failures\n", minunit_run, minunit_assert, minunit_fail);\
        double minunit_end_real_timer = mu_timer_real();\
        double minunit_end_proc_timer = mu_timer_cpu();\
        printf("\nFinished in %.8f seconds (real) %.8f seconds (proc)\n\n",\
                minunit_end_real_timer - minunit_real_timer,\
                minunit_end_proc_timer - minunit_proc_timer);\
)

// Assertions
#define mu_check(test) MU__SAFE_BLOCK(\
        minunit_assert++;\
        if (!(test)) {\
                snprintf(minunit_last_message, MINUNIT_MESSAGE_LEN, "%s failed:\n\t%s:%d: %s", __func__, __FILE__, __LINE__, #test);\
                minunit_status = 1;\
                return;\
        } else {\
                printf(".");\
        }\
)

#define mu_fail(message) MU__SAFE_BLOCK(\
        minunit_assert++;\
        snprintf(minunit_last_message, MINUNIT_MESSAGE_LEN, "%s failed:\n\t%s:%d: %s", __func__, __FILE__, __LINE__, message);\
        minunit_status = 1;\
        return;\
)

#define mu_assert(test, message) MU__SAFE_BLOCK(\
        minunit_assert++;\
        if (!(test)) {\
                snprintf(minunit_last_message, MINUNIT_MESSAGE_LEN, "%s failed:\n\t%s:%d: %s", __func__, __FILE__, __LINE__, message);\
                minunit_status = 1;\
                return;\
        } else {\
                printf(".");\
        }\
)

#define mu_assert_int_eq(expected, result) MU__SAFE_BLOCK(\
        minunit_assert++;\
        int minunit_tmp_e = (expected);\
        int minunit_tmp_r = (result);\
        if (minunit_tmp_e != minunit_tmp_r) {\
                snprintf(minunit_last_message, MINUNIT_MESSAGE_LEN, "%s failed:\n\t%s:%d: %d expected but was %d", __func__, __FILE__, __LINE__, minunit_tmp_e, minunit_tmp_r);\
                minunit_status = 1;\
                return;\
        } else {\
                printf(".");\
        }\
)

#define mu_assert_double_eq(expected, result) MU__SAFE_BLOCK(\
        minunit_assert++;\
        double minunit_tmp_e = (expected);\
        double minunit_tmp_r = (result);\
        if (fabs(minunit_tmp_e-minunit_tmp_r) > MINUNIT_EPSILON) {\
                snprintf(minunit_last_message, MINUNIT_MESSAGE_LEN, "%s failed:\n\t%s:%d: %g expected but was %g", __func__, __FILE__, __LINE__, minunit_tmp_e, minunit_tmp_r);\
                minunit_status = 1;\
                return;\
        } else {\
                printf(".");\
        }\
)

double mu_timer_real( )
{
        return -1.0;            /* Failed. */
}

/**
 * Returns the amount of CPU time used by the current process,
 * in seconds, or -1.0 if an error occurred.
 */
double mu_timer_cpu( )
{
        return -1;              /* Failed. */
}

#ifdef __cplusplus
}
#endif

#endif /* __MINUNIT_H__ */

