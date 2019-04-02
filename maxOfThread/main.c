#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *th_fuc(void *p)
{
  while(1)
  {
    sleep(1);
  }
  return (void *)1;
}
int main()
{
  pthread_t tid;
  int num = 0;
  int err = 0;
  while(1)
  {
    err = pthread_create(&tid, NULL, th_fuc, NULL);
    if(err != 0)
    {
      printf("%s\n", strerror(err));
      exit(1);
    }
    printf("%d\n", ++num);
  }
  return 0;
}
