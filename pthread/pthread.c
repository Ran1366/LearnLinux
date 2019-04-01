#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void *do_tid(void *n)
{
  int *p = (int *)n;
  printf("pthread %d\n", *p);
  printf("pthread %lu\n", pthread_self());
  printf("pid%d\n", getpid());
  return p;
}
int main()
{
  pthread_t tid;
  int n = 10;
  pthread_create(&tid, NULL, do_tid, &n);
  sleep(2);
  printf("main child pthread %lu\n", tid);
  printf("main pthread %d\n", n);
  printf("main pthread %lu\n", pthread_self());
  printf("main pid%d\n", getpid());
  return 0;
}
