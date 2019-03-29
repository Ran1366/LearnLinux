#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void do_sig(int a)
{
    //do nothing
}
unsigned int mysleep(unsigned int seconds)
{
  //先将SIGALRM信号阻塞, 再调用alarm计时, 将挂起和
  //设置SIGALRM信号非阻塞捆绑为原子操作(sigsuspend)
  //再把原来的阻塞信号集注册回去
  struct sigaction newact, oldact;
  sigset_t newmask, oldmask, susmask;
  unsigned int unslept;
  
  newact.sa_handler = do_sig;
  sigemptyset(&newact.sa_mask);
  newact.sa_flags = 0;
  sigaction(SIGALRM, &newact, &oldact);
  
  sigemptyset(&newmask);
  sigaddset(&newmask, SIGALRM);
  sigprocmask(SIG_BLOCK, &newmask, &oldmask);

  alarm(seconds);

  susmask = oldmask;
  sigdelset(&susmask, SIGALRM);
  
  sigsuspend(&susmask);
  
  unslept = alarm(0);
  sigaction(SIGALRM, &oldact, NULL);
  sigprocmask(SIG_SETMASK, &oldmask, NULL);
  
  return unslept;
}
int main()
{
  while(1)
  {
    mysleep(1);
    printf("sleep\n");
  }
  return 0;
}
