#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void act(int num)
{
  printf("i'm act\n");
  printf("num = %d\n", num);
}

int main()
{
  struct sigaction sig;
  sig.sa_handler = act;
  sig.sa_flags = 0;
  sigemptyset(&sig.sa_mask);
  sigaction(SIGINT, &sig, NULL);
  while(1)
  {
    printf("===\n");
    sleep(1);
  }
  return 0;
}
