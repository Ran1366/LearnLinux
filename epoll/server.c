#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#define SOCK_PORT 5000
#define MAX_EPOLL_NUM 1024
int main()
{
  struct sockaddr_in server, client;
  int listenfd, confd, client_num;
  char buf[INET_ADDRSTRLEN];
  socklen_t len;
  int i = 0;
  int clients[MAX_EPOLL_NUM];
  int epfd;
  struct epoll_event tep, ep[MAX_EPOLL_NUM]; 
  listenfd = socket(AF_INET, SOCK_STREAM, 0);

  //初始化
  bzero(&server, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  server.sin_port = htons(SOCK_PORT);


  bind(listenfd, (struct sockaddr*)&server, sizeof(server));
  
  listen(listenfd, 128);
  //创建一个epoll句柄
  epfd = epoll_create(MAX_EPOLL_NUM);

  //监控listenfd
  tep.events = EPOLLIN; 
  tep.data.fd = listenfd;
  epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &tep);
  while(1)
  {
    client_num = epoll_wait(epfd, ep, MAX_EPOLL_NUM, -1);
    //出错处理
    if(client_num < 0)
    {
      printf("epoll failed\n");
      exit(1);
    }
    //如果监听到有新的客户端请求
    for(i = 0; i < client_num; ++i)
    {
      if(!(ep[i].events & EPOLLIN))
      {
        continue;
      }
      if(ep[i].data.fd == listenfd)
      {
        len = sizeof(client);
        confd = accept(listenfd, (struct sockaddr*)&client, &len); 
        printf("client IP: %s, port: %d\n",
                inet_ntop(AF_INET, &client.sin_addr.s_addr, buf, sizeof(buf)),
                ntohs(client.sin_port));

        for(i = 0; i < MAX_EPOLL_NUM; ++i)
        {
          if(clients[i]  < 0)
          {
            clients[i] = confd;
            break;
          }
        }

        if(i == MAX_EPOLL_NUM)
        {
          printf("can't accept nomore clients\n");
          exit(1);
        }
        //添加新的文件描述符
        tep.events = EPOLLIN; 
        tep.data.fd = confd;
        epoll_ctl(epfd, EPOLL_CTL_ADD, confd, &tep);
        if(--len == 0)
        {
          continue;
        }
      }else{
        confd = ep[i].data.fd;
        int n = 0;
        //如果客户端关闭, 那么在服务器端也移除对应的文件描述符
        if((n = read(confd, buf, sizeof(buf))) == 0)
        {
          epoll_ctl(epfd, EPOLL_CTL_DEL, confd, NULL);
          close(confd);
        }
        else{
          int j;
          for(j = 0; j < n; ++j)
          {
            buf[j] = toupper(buf[j]);
          }
          write(confd, buf, n);
        }
      }
    }
  }
  close(listenfd);
}
