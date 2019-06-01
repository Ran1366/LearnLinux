#include "udp_socket.hpp"
#include <iostream>

#define CHECK_RIGHT(a) if((a) == false){return -1;}
int main(int argc, char *argv[])
{
  if(argc != 3)
  {
    perror("server ip port");
    return -1;
  }
  std::string ip = argv[1]; 
  uint16_t port = atoi(argv[2]);

  UdpSocket sock;
  sock.Socket();
  sock.Bind(ip, port);
  while(1)
  {
    std::string buf;
    struct sockaddr_in cli_addr;
    CHECK_RIGHT(sock.Recv(buf, &cli_addr));
    std::cout << "client: " << buf << std::endl;
    std::cout << "server: ";
    fflush(stdout);
    std::cin >> buf;
    CHECK_RIGHT(sock.Send(buf, &cli_addr));
  }
  sock.Close();
}
