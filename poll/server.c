#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/poll.h>

#define SOCK_PORT 5000
#define MAX_POLL_NUM 1024
int main()
{
  struct sockaddr_in server, client;
  int listenfd, confd, maxIdx, client_num;
  char buf[INET_ADDRSTRLEN];
  socklen_t len;
  int i = 0;
  struct pollfd clients[MAX_POLL_NUM];
  listenfd = socket(AF_INET, SOCK_STREAM, 0);

  //初始化
  bzero(&server, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  server.sin_port = htons(SOCK_PORT);


  bind(listenfd, (struct sockaddr*)&server, sizeof(server));
  
  listen(listenfd, 128);
  //监听普通读事件
  clients[0].fd = listenfd;
  clients[0].events = POLLRDNORM;

  for(i = 1; i < MAX_POLL_NUM; ++i)
  {
    clients[i].fd = -1;
  }
  //初始化最大文件描述符和其下标
  maxIdx = 0;
  while(1)
  {
    client_num = poll(clients, maxIdx + 1, -1);
    //出错处理
    if(client_num < 0)
    {
      printf("select failed\n");
      exit(1);
    }
    //如果监听到有新的客户端请求
    if(clients[0].revents & POLLRDNORM)
    {
      len = sizeof(client);
      confd = accept(listenfd, (struct sockaddr*)&client, &len); 
      printf("client IP: %s, port: %d\n",
              inet_ntop(AF_INET, &client.sin_addr.s_addr, buf, sizeof(buf)),
              ntohs(client.sin_port));

      for(i = 1; i < MAX_POLL_NUM; ++i)
      {
        if(clients[i].fd  < 0)
        {
          clients[i].fd = confd;
          break;
        }
      }

      if(i == MAX_POLL_NUM)
      {
        printf("can't accept nomore clients\n");
        exit(1);
      }
      //添加新的文件描述符到文件描述符集合中
      clients[i].events = POLLRDNORM;
      //更新最大文件描述符的下标
      if(i > maxIdx)
      {
        maxIdx = i;
      }
      if(--len == 0)
      {
        continue;
      }
    }
    //如果客户端发来数据,使用专门用于与客户端通信的文件描述符处理数据
    for(i = 0; i <= maxIdx; ++i)
    {
      if((confd = clients[i].fd) < 0)
      {
        continue;
      }
      if(clients[i].revents & (POLLRDNORM))
      {
        int n = 0;
        //如果客户端关闭, 那么在服务器端也移除对应的文件描述符
        if((n = read(confd, buf, sizeof(buf))) == 0)
        {
          close(confd);
          clients[i].fd = -1;
        }
        else{
          int j;
          for(j = 0; j < n; ++j)
          {
            buf[j] = toupper(buf[j]);
          }
          write(confd, buf, n);
        }
        if(--len == 0)
        {
          continue;
        }
      }
    }
  }
  close(listenfd);
}
