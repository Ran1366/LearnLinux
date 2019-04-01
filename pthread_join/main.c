#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *thread1(void *p)
{
  printf("i'm thread1\n");
  return (void *)1;
}

void *thread2(void *p)
{
  printf("i'm thread2\n");
  pthread_exit((void *)2);
}

void *thread3(void *p)
{
  while(1)
  {
    printf("i'm thread3\n");
    sleep(1);
  }
  return p;
}

int main()
{
  pthread_t tid;
  void *retval;
  pthread_create(&tid, NULL, thread1, NULL);
  pthread_join(tid, &retval);
  printf("thread1 retval = %d\n", (int)retval);
  
  pthread_create(&tid, NULL, thread2, NULL);
  pthread_join(tid, &retval);
  printf("thread2 retval = %d\n", (int)retval);

  pthread_create(&tid, NULL, thread3, NULL);
  sleep(3);
  pthread_cancel(tid); 
  pthread_join(tid, &retval);
  printf("thread3 retval = %d\n", (int)retval);
  
  
  return 0;
}
