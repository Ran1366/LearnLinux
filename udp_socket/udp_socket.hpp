//用C++将常用的udp套接字编程接口再封装
#pragma once 
#include <iostream>
#include <string>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

const int MAX_RECVDATA = 1500;

class UdpSocket 
{
public:
  UdpSocket():sock_fd(-1){}
  //int socket(int domain, int type, int protocol);
  bool Socket()
  {
    if((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
      perror("Socket erro");
      return false;
    }
    return true;
  }

  bool Bind(std::string ip, uint16_t port) 
  {
    struct sockaddr_in addr;
    bzero(&addr, sizeof(sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    socklen_t len = sizeof(addr);
    //int bind(int sockfd, const struct sockaddr *addr,socklen_t addrlen);
    if(bind(sock_fd, (struct sockaddr*)&addr, len) < 0)
    {
      perror("bind erro");
      return false;
    }
    return true;
  }

  //ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,struct sockaddr *src_addr, socklen_t *addrlen);
  bool Recv(std::string &data, struct sockaddr_in *saddr)
  {
    char tmp[MAX_RECVDATA] = {0};
    socklen_t len = sizeof(saddr);
    int ret = recvfrom(sock_fd, tmp, MAX_RECVDATA, 0, (struct sockaddr *)saddr, &len);
    if(ret < 0)
    {
      perror("Recv erro");
      return false;
    }
    data.assign(tmp, ret);
    return true;
  }

  //    ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
  //                        const struct sockaddr *dest_addr, socklen_t addrlen);
  bool Send(std::string &data, struct sockaddr_in *daddr)
  {
    socklen_t len = sizeof(struct sockaddr_in);
    if(sendto(sock_fd, data.c_str(), data.size(), 0, (struct sockaddr*)daddr, len) < 0)
    {
      perror("Send erro");
      return false;
    }
    return true;
  }
 
  bool Close()
  {
    close(sock_fd);
    sock_fd = -1;
    return true;
  }
private:
  int sock_fd;
};
