#include <iostream>
#include "tcp_socket.hpp"

int main(int argc, char *argv[])
{
  if (argc != 3 ) {
    std::cout<<"./tcp_cli ip port\n";
    return -1;
  }
  std::string ip = argv[1];
  uint16_t port = atoi(argv[2]);

  tcp_socket sock;
  CHECK_RIGHT(sock.Socket());
  CHECK_RIGHT(sock.Connect(ip, port));

  while(1) {
    std::string buf;
    std::cout<<"client say:";
    fflush(stdout);
    std::cin>>buf;
    sock.Send(buf);
  }
  sock.Close();
  return 0;
}
