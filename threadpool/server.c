#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ctype.h>
#include "threadpool.h"

typedef struct s_info 
{
  struct sockaddr_in client_addr;
  int confd;
}s_info;
#define SERVER_PORT 7000
void *do_work(void *p)
{
  int i, len;
  char addbuf[128];
  char buf[1024];
  s_info *info = (s_info*)p; 
  printf("client ip:%s, port:%d\n", 
      inet_ntop(AF_INET, &info->client_addr.sin_addr.s_addr, addbuf, (socklen_t)sizeof(addbuf)),
      ntohs(info->client_addr.sin_port) 
      );
  while(1)
  {
    len = read(info->confd, buf, sizeof(buf));
    i = 0;
    while(i < len)
    {
      buf[i] = toupper(buf[i]); 
      ++i;
    }
    buf[i] = '\0';
    if(strcmp(buf, "EOF\n") == 0)
    {
      break;
    }
    write(info->confd, buf, len);
  }
  close(info->confd);
  return (void*)1;

}
int main()
{
  struct sockaddr_in server_addr, client_addr;
  int sockfd, addlen, confd; 
  s_info s;
  thread_pool *pool = thread_pool_create(10, 20, 15);
  //socket
  sockfd = socket(AF_INET, SOCK_STREAM, 0); 
  //bzero
  bzero(&server_addr, sizeof(server_addr));
  //set protocol ipv4
  server_addr.sin_family = AF_INET;
  //set ip address
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  //set port number
  server_addr.sin_port = htons(SERVER_PORT);
  //bind the port number
  bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  //listen
  listen(sockfd, 128);
  while(1){
    //get len of the client_addr 
    addlen = sizeof(client_addr);
    //listen the client's connection
    confd = accept(sockfd, (struct sockaddr*)&server_addr, (socklen_t*)&addlen);
    s.confd = confd;
    s.client_addr = client_addr;
    thread_pool_addtask(pool, do_work, (void*)&s);
  }
  close(sockfd);
  return 0;
}
