#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void pcount(int n, siginfo_t *sig, void *ptr)
{
  static int num = 0;
  printf("i'm parent now is %d\n",num);
  num += 2;
  sleep(1);
  kill(sig->si_pid, SIGUSR2);
}
void ccount(int n, siginfo_t *sig, void *ptr)
{
  static int num = 1;
  printf("i'm child now is %d\n",num);
  num += 2;
  sleep(1);
  kill(sig->si_pid, SIGUSR1);
}
int main()
{
  pid_t pid = fork();
  struct sigaction act;
  if(pid > 0)
  {
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    //act.sa_restorer = pcount;
    act.sa_sigaction= pcount;
    sigaction(SIGUSR1, &act, NULL);
    while(1)
    {
      sleep(1);
    }
  }
  if(pid == 0)
  {
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    //act.sa_restorer = ccount;
    act.sa_sigaction = ccount;
    sigaction(SIGUSR2, &act, NULL);
    pid_t ppid = getppid();
    kill(ppid, SIGUSR1);
    while(1)
    {
      sleep(1);
    }
  }

  return 0;
}
