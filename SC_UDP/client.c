#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define SERVER_PORT 8000
int main(int argc, char *argv[])
{
  if(argc < 2)
  {
    printf("./client str\n");
    exit(1);
  }
  struct sockaddr_in server_addr;
  char server_ip[] = "192.168.197.133";
  int confd;
  char buf[1024];
  socklen_t server_addr_len;
  int len;

  confd = socket(AF_INET, SOCK_DGRAM, 0);
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  inet_pton(AF_INET, server_ip, &server_addr.sin_addr.s_addr);
  server_addr.sin_port = htons(SERVER_PORT); 

  sendto(confd, argv[1], strlen(argv[1]), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

  server_addr_len = sizeof(server_addr);
  len = recvfrom(confd, buf, sizeof(buf), 0, NULL, 0);
  write(STDOUT_FILENO, buf, len);
  printf("\n");
  close(confd);
  return 0;
}
