//用C++封装TCP套接字编程的常用接口
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define CHECK_RIGHT(p) if((p) == false) {return -1;}
#define MAX_RECVDATA 4096

typedef struct Data 
{
  int num1;
  char Operator;
  int num2;
}Data;
class tcp_socket 
{
public:
  tcp_socket():_sock_fd(-1) {}
  int getSocketFd()
  {
    return _sock_fd;
  }
  void setSocketFd(int newFd)
  {
    _sock_fd = newFd;
  }
  //int socket(int domain, int type, int protocol);
  bool Socket()
  {
    _sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(_sock_fd < 0) 
    {
      perror("Socket erro");
      return false;
    }
    return true;
  }

  //int bind(int sockfd, const struct sockaddr *addr,
  //          socklen_t addrlen);
  bool Bind(const std::string &ip, uint16_t port)
  {
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    
    socklen_t len = sizeof(addr);
    if(bind(_sock_fd, (struct sockaddr*)&addr, len) < 0)
    {
      perror("Bind erro");
      return false;
    }
    return true;
  }

  //int listen(int sockfd, int backlog); 
  bool Listen(int backlog = 64)
  {
    if(listen(_sock_fd, backlog) < 0)
    {
      perror("listen erro");
      return false;
    }
    return true;
  }

  //int accept(int sockfd, struct sockaddr* addr,
  //        socklen_t* addrlen);
  bool Accept(tcp_socket &cSock, struct sockaddr_in *caddr = NULL)
  {
    struct sockaddr_in tmp_addr;
    socklen_t len = sizeof(tmp_addr);
    int new_fd = accept(_sock_fd, (struct sockaddr*)&tmp_addr, &len);
    if(new_fd < 0)
    {
      perror("accept erro");
      return false;
    }
    cSock.setSocketFd(new_fd);
    if(caddr != NULL)
    {
      memcpy(caddr, &tmp_addr, len );
    }
    return true;
  }

  //int connect(int sockfd, const struct sockaddr *addr,
  //          socklen_t addrlen);
  bool Connect(std::string &ip, uint16_t port)
  {
    struct sockaddr_in s_addr;
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(port);
    s_addr.sin_addr.s_addr = inet_addr(ip.c_str());

    socklen_t len = sizeof(sockaddr_in);
    if(connect(_sock_fd, (struct sockaddr*)&s_addr, len) < 0)
    {
      perror("Connect erro");
      return false;
    }
    return true;
  }

  //ssize_t recv(int sockfd, void *buf, size_t len, int flags)
  bool Recv(std::string &buf)
  {
    char tmp[MAX_RECVDATA] = {0};
    int ret = recv(_sock_fd, tmp, MAX_RECVDATA, 0);
    if(ret < 0)
    {
      perror("recv erro");
      return false;
    }
    else if(ret  == 0)
    {
      printf("client disconnect\n");
      return false;
    }
    buf.assign(tmp, ret);
    return true;
  }

  //ssize_t send(int sockfd, void *buf, size_t len, int flags)
  bool Send(std::string &buf)
  {
    if(send(_sock_fd, buf.c_str(), buf.size(), 0) < 0)
    {
      perror("send erro"); 
      return false;
    }
    return true;
  }

  bool Close()
  {
    close(_sock_fd);
    _sock_fd = -1;
    return true;
  }
private:
  int _sock_fd;
};
