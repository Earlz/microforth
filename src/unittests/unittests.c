/**
<Copyright Header>
Copyright (c) 2013 Jordan "Earlz" Earls  <http://Earlz.net>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1. Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.
3. The name of the author may not be used to endorse or promote products
  derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL
THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
</Copyright Header>
**/

#pragma GCC diagnostic ignored "-Wunused-variable"
#include "minunit.h"
#include <stdlib.h>
#include <string.h>

#include "../microforth.h"


#define assert mu_check

MU_TEST(test_push_pop_works)
{
  mf_state_t mf;
  mf_init(&mf, 100);
  mf_push(&mf, 0x1234);
  assert(mf_pop(&mf) == 0x1234);
  
  mf_push(&mf, 0x4567);
  mf_push(&mf, 0xabcd);
  mf_push(&mf, 0xff23);
  assert(mf_pop(&mf)==0xff23);
  assert(mf_pop(&mf)==0xabcd);
  assert(mf_pop(&mf)==0x4567);
  mf_destroy(&mf);
}

MU_TEST(test_push_pop_proper_errors)
{
  mf_state_t mf;
  mf_init(&mf, 100);
  mf.stacklen=1;
  mf_push(&mf, 1);
  assert(mf.error==0);
  mf_push(&mf, 1);
  assert(mf.error!=0 && mf.error_msg!=NULL);
  mf.error=0;
  mf_pop(&mf);
  assert(mf.error==0);
  mf_pop(&mf);
  assert(mf.error!=0);
  mf_destroy(&mf);
}


MU_TEST_SUITE(test_suite) {
    MU_RUN_TEST(test_push_pop_works);
    MU_RUN_TEST(test_push_pop_proper_errors);
}


int main(int argc, char *argv[]) {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return 0;
}

