#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *th_fuc(void *p)
{
  int n = 10;
  while(n--)
  {
    printf("now is %d\n", n);
    sleep(1);
  }
  return (void*)1;
}
int main()
{
  pthread_t tid;
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  
  pthread_create(&tid, &attr, th_fuc, NULL);
  
  int err = pthread_join(tid, NULL);
  while(1)
  {
    if(err != 0)
    {
      printf("%s\n", strerror(err));
    }
    sleep(5);
    pthread_exit((void*)1);
  }
  return 0;
}
