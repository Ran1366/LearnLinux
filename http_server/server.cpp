#include "tcp_socket.hpp"
#include <iostream>
#include <sstream>
#include <pthread.h>

void* th_fuc(void *data)
{
  tcp_socket *clisock = (tcp_socket*)data;
  std::string buf;
  clisock->Recv(buf);
  std::cout << "req:[" << buf << "]\n"; 

  std::string body;
  body = "<html><body><h1>HELLO WORLD</h1></body></html>";

  std::stringstream ss;
  ss << "HTTP/1.1 200 OK \r\n";
  ss << "Content-Length: " << body.size() << "\r\n";
  ss << "Content-Type: text/html\r\n";
  ss << "\r\n";
  std::string header = ss.str();

  clisock->Send(header);
  clisock->Send(body);
  clisock->Close();
  return (void*)0;
}

int main(int argc, char *argv[])
{
  if(argc != 3)
  {
    perror("./server ip port");
    return -1;
  }
  tcp_socket sock;
  std::string ip = argv[1];
  uint16_t port = atoi(argv[2]);
  CHECK_RIGHT(sock.Socket());
  CHECK_RIGHT(sock.Bind(ip, port));
  CHECK_RIGHT(sock.Listen());
  
  pthread_t tid;
  while(1)
  {
    tcp_socket *clisock = new tcp_socket;
    if(sock.Accept(*clisock) == false)
    {
      continue;
    }
    else 
    {
      pthread_create(&tid, NULL, th_fuc, (void*)clisock); 
      pthread_detach(tid);
    }
  }
  sock.Close();
}
