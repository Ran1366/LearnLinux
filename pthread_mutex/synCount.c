#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX 1000
unsigned int num;

pthread_mutex_t lock;
void *count1(void *p)
{
  int i = 0;
  for(; i < MAX; ++i)
  {
    pthread_mutex_lock(&lock);
    printf("count1 %d\n", ++num);
    pthread_mutex_unlock(&lock);
    usleep(100);
  }
  return (void *)1;
}

void *count2(void *p)
{
  int i = 0;
  for(; i < MAX; ++i)
  {
    pthread_mutex_lock(&lock);
    printf("count2 %d\n", ++num);
    pthread_mutex_unlock(&lock);
    usleep(100);
  }
  return (void *)1;
}
int main()
{
  pthread_t tid1, tid2;
  pthread_mutex_init(&lock, NULL);

  pthread_create(&tid1, NULL, count1, NULL);
  pthread_create(&tid2, NULL, count2, NULL);
  
  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);
  return 0;
}
