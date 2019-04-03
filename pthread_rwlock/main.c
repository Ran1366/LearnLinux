#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

unsigned int num;
pthread_rwlock_t lock;
void *th_read(void *p)
{
  while(1)
  {
    //读锁
    pthread_rwlock_rdlock(&lock);
    printf("th_read %lu %d\n", pthread_self(), num);
    //解锁
    pthread_rwlock_unlock(&lock);
    usleep(1000);
  }
  return NULL;
}

void *th_write(void *p)
{
  while(1)
  {
    //写锁
    pthread_rwlock_wrlock(&lock);
    ++num;
    printf("th_write %lu %d\n", pthread_self(), num);
    //解锁
    pthread_rwlock_unlock(&lock);
    usleep(1000);
  }
  return NULL;
}
int main()
{
  pthread_t tid[8];
  int i = 0;
  //初始化读写锁
  pthread_rwlock_init(&lock, NULL);
  //创建三个写线程
  for(i = 0; i < 3; ++i) 
  {
    pthread_create(&tid[i], NULL, th_write, NULL);
  }
  //创建五个读线程
  for(i = 3; i < 8; ++i) 
  {
    pthread_create(&tid[i], NULL, th_read, NULL);
  }
  //销毁读写锁
  pthread_rwlock_destroy(&lock);
  //回收所有线程
  for(i = 0; i < 8; ++i)
  {
    pthread_join(tid[i], NULL);
  }
  return 0;
}
