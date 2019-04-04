#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "list.h"

pthread_cond_t cond;
pthread_mutex_t lock;
Node *head = NULL;


//typedef struct List 
//{
//  int size;
//  int capcity;
//  Node *head;
//}List;
//List list = {10, 100, NULL};
void *do_pro(void *p)
{
  while(1) 
  {
    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->val = rand()%1000;
    //生产者不断地生产(头插操作)
    pthread_mutex_lock(&lock);//加锁
    newNode->Next = head;
    head = newNode;
    printf("producer %lu %d\n", pthread_self(), head->val);
    pthread_mutex_unlock(&lock);//解锁
    //通知阻塞的消费者线程, 让其解除阻塞
    pthread_cond_signal(&cond);
    sleep(rand() % 3);
  }
  return NULL;
}

void *do_cus(void *p)
{
  while(1) 
  {
    
    pthread_mutex_lock(&lock); //加锁
    //如果当前链表为空, 消费者阻塞等待
    if(head == NULL)
    {
      //这个函数会锁定互斥锁
      pthread_cond_wait(&cond, &lock);
    }
    //消费者不断地消费(头删操作)
    Node *deleted = head;
    head = head->Next;
    deleted->Next = NULL;
    printf("customer%lu %d\n", pthread_self(), deleted->val);
    pthread_mutex_unlock(&lock);//解锁
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

  //初始化锁和条件变量
  pthread_mutex_init(&lock, NULL);
  pthread_cond_init(&cond, NULL);

  //创建生产者线程
  pthread_create(&producer, NULL, do_pro, NULL);
  //创建消费者线程
  pthread_create(&customer, NULL, do_cus, NULL);
  
  //回收线程
  pthread_join(producer, NULL);
  pthread_join(customer, NULL);
  //销毁锁和条件变量
  pthread_mutex_destroy(&lock);
  pthread_cond_destroy(&cond);
  return 0;
}
