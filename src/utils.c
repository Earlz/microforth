#include "config.h"
#include "microforth.h"


int strlcmpt(const char* str1, const char* str2, bool (*terminating)(char c))
{
  int i=0;
  while(!terminating(str1[i]) && !terminating(str2[i]))
  {
    if(str1[i] != str2[i])
    {
      return 1;
    }
    i++;
  }
  return 0;
}