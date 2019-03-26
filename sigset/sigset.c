#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void printSigsetno(const sigset_t *set)
{
  int i = 1;
  for(; i < 32; ++i)
  {
    if(sigismember(set, i) == 1)
    {
      printf("1");
    }
    else{
      printf("0");
    }
  }
  printf("\n");
}
int main()
{
  sigset_t s, p;
  sigemptyset(&s);
  sigaddset(&s, SIGINT);
  sigaddset(&s, SIGQUIT);
  sigprocmask(SIG_SETMASK, &s, NULL);
  int i = 0;
  while(++i)
  {
    sigpending(&p);
    printSigsetno(&p);
    if(i == 5)
    {
      sigdelset(&s, SIGQUIT);
      sigprocmask(SIG_UNBLOCK, &s, NULL);
    }
    sleep(1);    
  }
  return 0;
}
