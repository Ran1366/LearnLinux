#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  struct sockaddr_in server_addr;
  int confd, len;
  char ipstr[] = "192.168.197.133";
  char buf[1024];
  if(argc < 2)
  {
    printf("./client str\n");
    exit(1);
  }
  //socket
  confd = socket(AF_INET, SOCK_STREAM, 0);

  //set ip address and port 
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(8000);
  inet_pton(AF_INET, ipstr, &server_addr.sin_addr.s_addr);
  
  //connect with server
  connect(confd, (struct sockaddr*)&server_addr, (socklen_t)sizeof(server_addr));

  //Request the server to process the data
  write(confd, argv[1], strlen(argv[1])); 
  len = read(confd, buf, sizeof(buf));
  write(STDOUT_FILENO, buf, len);
  printf("\n");
  //close the socket
  close(confd);
  return 0;
}
