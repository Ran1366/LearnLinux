#include "udp_socket.hpp"
#include <iostream>

#define CHECK_RIGHT(p) if((p) == false){return -1;}
int main(int argc, char *argv[])
{
  if(argc != 3)
  {
    perror("./cleant ip port");
    return -1;
  }

  std::string ip = argv[1];
  uint16_t port = atoi(argv[2]);
  UdpSocket sock;
  CHECK_RIGHT(sock.Socket());

  struct sockaddr_in s_addr;
  s_addr.sin_family = AF_INET;
  s_addr.sin_port = htons(port);
  s_addr.sin_addr.s_addr = inet_addr(ip.c_str());

  while(1)
  {
    std::string buf;
    std::cout << "client: ";
    fflush(stdout);
    std::cin >> buf;
    CHECK_RIGHT(sock.Send(buf, &s_addr)); 
      
    CHECK_RIGHT(sock.Recv(buf, &s_addr));
    std::cout << "server: " << buf << std::endl;
  }
  return 0;
}
