#include "config.h"
#include "microforth.h"


void mf_push(mf_state_t* state, intptr_t value)
{
  if(state->stackpos>=state->stacklen)
  {
    state->error=1;
    state->error_msg="Stack Overflow";
    return;
  }
  state->stack[state->stackpos++]=value;
}
intptr_t mf_pop(mf_state_t* state)
{
  if(state->stackpos==0)
  {
    state->error=1;
    state->error_msg="Stack Underflow";
    return -1;
  }
  return state->stack[--state->stackpos];
}

int mf_init(mf_state_t* state, int stacksize)
{
  state->stack=malloc(stacksize*sizeof(intptr_t*));
  if(state->stack==NULL)
  {
    return -1;
  }
  state->stackpos=0;
  state->stacklen=stacksize;
  state->error=0;
  state->error_msg=NULL;
}

int mf_destroy(mf_state_t* state)
{
  if(state->stack!=NULL)
  {
    free(state->stack);
  }
  return 0;
}