#include <unistd.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

typedef struct Node 
{
  int val;
  struct Node *Next;
}Node;
sem_t producer_sem;
sem_t customer_sem;
Node *head = NULL;


void *do_pro(void *p)
{
  while(1) 
  {
    sem_wait(&producer_sem);
    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->val = rand()%1000;
    //生产者不断地生产(头插操作)
    newNode->Next = head;
    head = newNode;
    printf("+++producer %lu %d\n", pthread_self(), head->val);
    //通知阻塞的消费者线程, 让其解除阻塞
    sem_post(&customer_sem);
    sleep(rand() % 3);
  }
  return NULL;
}

void *do_cus(void *p)
{
  while(1) 
  {
    
    //如果当前链表为空, 消费者阻塞等待
    sem_wait(&customer_sem);
    //消费者不断地消费(头删操作)
    Node *deleted = head;
    head = head->Next;
    deleted->Next = NULL;
    printf("---customer %lu %d\n", pthread_self(), deleted->val);
    sem_post(&producer_sem);
    free(deleted);//释放已被删除的结点
    sleep(rand() % 3);
  }
  return NULL;
}
int main()
{
  //时间种子
  srand((unsigned)time(0));
  //生产者&消费者模型
  pthread_t producer, customer;

  //初始化锁
  sem_init(&producer_sem, 0, 4);
  sem_init(&customer_sem, 0, 0);
  //创建生产者线程
  pthread_create(&producer, NULL, do_pro, NULL);
  //创建消费者线程
  pthread_create(&customer, NULL, do_cus, NULL);
  
  //回收线程
  pthread_join(producer, NULL);
  pthread_join(customer, NULL);
  //销毁锁
  sem_destroy(&producer_sem);
  sem_destroy(&customer_sem);
  return 0;
}
