#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SERVER_PORT 8000
int main()
{
  struct sockaddr_in server_addr, client_addr;
  int sockfd, addlen, confd, len, i;
  char addbuf[128];
  char buf[1024];
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

    //Create a child process to deal with client requests
    int pid = fork();
    if(pid == 0)
    {
      //in child process
      close(sockfd);
      printf("client ip:%s, port:%d\n", 
              inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, addbuf, (socklen_t)sizeof(addbuf)),
              ntohs(client_addr.sin_port) 
              );
      while(1)
      {
        len = read(confd, buf, sizeof(buf));
        i = 0;
        while(i < len)
        {
          buf[i] = toupper(buf[i]); 
          ++i;
        }
        write(confd, buf, len);
      }
      close(confd);
    }
    else if(pid > 0)
    {
      //in parent process
      close(confd);
    }
  }
  close(sockfd);
  return 0;
}
