#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

//void*类型参数：用于传递消息处理函数需要的信息
typedef struct thread_task
{
  void *(*process)(void *arg);
  void *arg;
}thread_task;

typedef struct thread_pool
{
  //线程池状态信息：描述当前线程池的基本信息，如是否开启、最小线程数、最大线程数、存活线程数、忙线程数、待销毁线程数等… … 
  int iswork;//1表示开启,0表示关闭
  int min_thread_num;//最小线程数
  int max_thread_num;//最大线程数
  int live_thread_num;//存活线程数
  int busy_thread_num;//忙线程数
  int tobedes_thread_num;//待销毁线程数
  //任务队列信息：描述当前任务队列基本信息，如最大任务数、队列不为满条件变量、队列不为空条件变量等… … 
  int max_queue_task;//最大任务数
  int cur_queue_task;//当前任务数
  int front_queue_task;//任务队列首
  int rear_queue_task;//任务队列尾
  pthread_cond_t queue_not_full;//队列不为满条件变量
  pthread_cond_t queue_not_empty;//队列不为空条件变量
  //多线程互斥锁：保证在同一时间点上只有一个线程在任务队列中取任务并修改任务队列信息、修改线程池信息； 
  pthread_mutex_t cond_lock;
  pthread_mutex_t thread_lock;
  pthread_mutex_t task_lock;

  thread_task *queue_task;//任务队列  
  pthread_t *threadid;//线程数组,存储线程池中的线程id
}thread_pool;

//线程池的线程竞争任务
void* thread_pool_compete(void *threadpool)
{
  printf("thread_id : %lu waiting. . .\n", pthread_self());
  thread_pool *pool = (thread_pool*)threadpool;
  thread_task *task = (thread_task*)malloc(sizeof(thread_task));
  //加锁
  pthread_mutex_lock(&pool->thread_lock);
  //如果当前任务队列不为空则所有线程都竞争任务
  do 
  {
    //如果当前任务队列为空,则所有线程都阻塞等待
    if(pool->iswork && pool->cur_queue_task == 0)
    {
      pthread_cond_wait(&pool->queue_not_empty, &pool->cond_lock);
    }
    printf("有一个线程抢到了任务\n");//日志
    //同一时刻只有一个线程领取任务,并修改数据
    task->process = pool->queue_task[pool->front_queue_task].process; 
    task->arg = pool->queue_task[pool->front_queue_task].arg;
    pool->cur_queue_task -= 1;
    pool->front_queue_task = (pool->front_queue_task + 1) % pool->max_queue_task;//与任务队列尾配合,形成逻辑环
    pool->busy_thread_num += 1;
    pthread_cond_broadcast(&(pool->queue_not_full));
    //解锁
    pthread_mutex_unlock(&pool->thread_lock);
    //执行任务
    printf("thread_id : %lu working. . .\n", pthread_self());
    printf("现在有%d个线程正忙\n", pool->busy_thread_num);
    (*task->process)(task->arg);
    pool->busy_thread_num -= 1;
    printf("thread_id : %lu finished work\n", pthread_self());

    //加锁
    //pthread_mutex_lock(&pool->thread_lock);
    //printf("现在有%d个线程正忙\n", pool->busy_thread_num);
    ////解锁
    //pthread_mutex_unlock(&pool->thread_lock);
  }while(pool->iswork && pool->cur_queue_task < pool->max_queue_task);
  return (void*)1;
}

//创建一个新的线程池
thread_pool *thread_pool_create(int min_thread, int max_thread, int max_task)
{
  do
  {
    thread_pool *pool = (thread_pool*)malloc(sizeof(thread_pool));
    if(pool == NULL)
    {
      printf("pool malloc failed\n");
      break;
    }
    pool->iswork = 1;
    pool->min_thread_num = min_thread;
    pool->max_thread_num = max_thread;
    pool->max_queue_task = max_task;
    pool->cur_queue_task = 0;
    pool->busy_thread_num = 0;
    pool->front_queue_task = 0;
    pool->rear_queue_task = 0;
    pool->tobedes_thread_num = 0;
    pool->live_thread_num = pool->min_thread_num;

    //给任务队列分配空间
    pool->queue_task = (thread_task*)malloc(sizeof(thread_task) * pool->max_queue_task);
    if(pool->queue_task == NULL)
    {
      printf("queue_task malloc failed\n");
      break;
    }
    //根据最大线程数给线程数组分配空间
    pool->threadid = (pthread_t*)malloc(sizeof(pthread_t) * pool->max_thread_num); 
    if(pool->threadid == NULL)
    {
      printf("threadid malloc failed\n");
      break;
    }
    //初始化锁
    pthread_mutex_init(&pool->cond_lock, NULL);
    pthread_mutex_init(&pool->thread_lock, NULL);
    pthread_mutex_init(&pool->task_lock, NULL);
    //初始化条件变量
    pthread_cond_init(&pool->queue_not_empty, NULL);
    pthread_cond_init(&pool->queue_not_full, NULL);
    //创建线程处理任务
    int i = 0;
    for(i = 0;i < pool->min_thread_num; ++i)
    {
      pthread_create(&pool->threadid[i], NULL, thread_pool_compete, (void*)pool);
      pool->live_thread_num += 1;
    }
    return pool;
  }while(0);
  //如果走到这一步说明动态申请内存失败,说明错误,返回NULL
  printf("create thread_pool failed\n");
  return NULL;
}

//当用户请求到来时, 主控线程把任务添加至任务队列
void thread_pool_addtask(thread_pool *threadpool, void*(*fuction)(void *arg), void*arg)
{
  thread_pool *pool = threadpool;
  //如果当前队列已满,则阻塞等待
  if(pool->cur_queue_task == pool->max_queue_task && pool->iswork)
  {
    pthread_cond_wait(&pool->queue_not_empty, &pool->cond_lock);
  }
  //加锁
  pthread_mutex_lock(&pool->task_lock);
  //任务队列不满则添加任务到队列中
  pool->queue_task[pool->front_queue_task].arg = arg;
  pool->queue_task[pool->front_queue_task].process = fuction;  
  pool->rear_queue_task = (pool->rear_queue_task + 1) % pool->max_queue_task;//与队首搭配构成逻辑环 
  pool->cur_queue_task += 1;
  //通知线程组有任务了
  printf("add task finish\n");//日志
  pthread_cond_signal(&pool->queue_not_empty);
  //解锁
  pthread_mutex_unlock(&pool->task_lock);
}

//销毁线程
void thread_pool_destroy(thread_pool *threadpool)
{
  thread_pool *pool = threadpool;
  if(pool->iswork)
  {
    pool->iswork = 0; 
  }
}

//管理者线程
void thread_pool_admin()
{

}
