#include <stdio.h>    
#include <unistd.h>    
#include <errno.h>    
#include <stdlib.h>    
int main()    
{    
  if((access("aaa", F_OK)) < 0)    
  {    
    perror("");    
    exit(1);    
  }    
  printf("aaa is ok\n");    
  return 0;                                                      
} 