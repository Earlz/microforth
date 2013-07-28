#ifndef MICROFORTH_H
#define MICROFORTH_H

#include <stdint.h>
#include <stdlib.h>


typedef struct{
  intptr_t* stack; //make sure the stack can store pointers
  size_t stackpos;
  size_t stacklen;
  
  int error;
  char* error_msg;
} mf_state_t;

typedef void (*mf_word_func_t)(mf_state_t* state);

typedef struct{
  char* name;
  char* body;
  mf_word_func_t internal_func;
} mf_word_t;

typedef struct mf_word_entry{
  mf_word_t word;
  struct mf_word_entry* next;
} mf_word_entry_t;

void mf_push(mf_state_t* state, int value);
int mf_pop(mf_state_t* state);


int mf_parse_word(mf_state_t* state, const char* word);
int mf_execute(mf_state_t* state, const  char* text);



#endif
