#include <string.h>
#include <stdio.h>
#include <stdlib.h>
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
  return 0;
}

int mf_destroy(mf_state_t* state)
{
  if(state->stack!=NULL)
  {
    free(state->stack);
  }
  return 0;
}


const char* skip_whitespace(const char* text)
{
  while(IS_WHITESPACE(*text) || *text==0)
  {
    text++;
  }
  return text;
}

int mf_execute(mf_state_t* state, const  char* text)
{
  const char* word=text;
  while(*word!=0)
  {
    word=skip_whitespace(text);
    word=mf_execute_word(state, word);
    if(state->error!=0)
    {
      fprintf(stderr, "Error at character '%il'. Message: '%s'\n",(unsigned int)(word-text), state->error_msg);
      return -1;
    }
  }
  return 0;
}




const char* mf_execute_word(mf_state_t* state, const char* word)
{
  if(*word==0) return word;
  char *end=NULL; 
  long result=strtol(word, &end, 10);
  if(end!=word && end!=NULL)
  {
    if(*end=='u')
    {
      result=(long)strtoul(word, &end, 10); //reparse as unsigned integer
    }
    mf_push(state, (intptr_t)result);
    return end;
  }
  //not a number. Must be an actual word!
  
  
  return end;
}








