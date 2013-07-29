#ifndef MICROFORTH_H
#define MICROFORTH_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define IS_WHITESPACE(c) (c==' ' || c=='\t' || c=='\r' || c=='\n')
#define IS_NUMBER(c) ((c>='0' && c<='9') || c=='-')

struct mf_state;

typedef void (*mf_word_func_t)(struct mf_state* state);

typedef struct{
  char* name;
  char* body;
  mf_word_func_t internal_func;
} mf_word_t;

typedef struct mf_word_entry{
  mf_word_t word;
  struct mf_word_entry* next;
} mf_word_entry_t;

typedef struct mf_state{
  intptr_t* stack; //make sure the stack can store pointers
  size_t stackpos;
  size_t stacklen;
  
  int error;
  const char* error_msg;
  mf_word_entry_t* words;
} mf_state_t;

void mf_push(mf_state_t* state, intptr_t value);
intptr_t mf_pop(mf_state_t* state);
int mf_init(mf_state_t* state, int stacksize);
int mf_destroy(mf_state_t* state);

const char* mf_execute_word(mf_state_t* state, const char* word);
int mf_execute(mf_state_t* state, const  char* text);


//utilities
/**
 * compares two strings. Both strings are deemed to terminate when the terminating function returns true
 * @str1 the first string to compare
 * @str2 the second string to compare 
 * @terminators the (null terminated) list of characters that can terminate the strings
 * @returns zero if equal(up to termination). non-zero if not equal
 */
//int strncmpt(const char* str1, const char* str2, char* terminators);



#endif
