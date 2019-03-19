#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main()
{
  int pid = fork();
  if(pid > 0)
  {
    while(1)
    {
      printf("i'm parent %d\n", getpid());
      int pid_c = wait(NULL);
      printf("wait for child %d\n", pid_c);
    }
  }
  if(pid == 0)
  {
    printf("i'm child %d\n", getpid());
    sleep(10);
  }
}
