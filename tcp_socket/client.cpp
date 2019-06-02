#include "tcp_socket.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
  if(argc != 3)
  {
    perror("./client ip port");
    return -1;
  }

  std::string ip = argv[1];
  uint16_t port = atoi(argv[2]);

  //创建套接字
  tcp_socket s_sock;
  CHECK_RIGHT(s_sock.Socket());
  //请求连接服务端
  CHECK_RIGHT(s_sock.Connect(ip, port));
  while(1)
  {
    std::string buf;
    std::cout << "client: " ;
    fflush(stdout); 
    getline(std::cin, buf);
    s_sock.Send(buf);
    
    s_sock.Recv(buf);
    std::cout << "server: " << buf << std::endl;
  }
  return 0;
}
