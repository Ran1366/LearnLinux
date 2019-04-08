#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define SERVER_PORT 8000
#define MAXLEN 1500
int main()
{
  struct sockaddr_in server_addr, client_addr;
  int sockfd, i, len;
  socklen_t client_addr_len; 
  char buf[MAXLEN];
  char ipstr[INET_ADDRSTRLEN];

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  bzero(&server_addr, sizeof(server_addr)); 
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(SERVER_PORT);

  bind(sockfd, (struct sockaddr*)&server_addr, (socklen_t)sizeof(server_addr));

  while(1)
  {
    client_addr_len = sizeof(client_addr); 
    len = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr*)&client_addr, &client_addr_len);
    printf("client ip = %s, port = %d\n",
            inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, ipstr, sizeof(ipstr)),
            ntohs(client_addr.sin_port)
          );
    i = 0; 
    while(i < len)
    {
      buf[i] = toupper(buf[i]);
      ++i;
    }
    buf[i] = '\0';
    sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr*)&client_addr, sizeof(client_addr));
  }
  close(sockfd);
  return 0;
}
