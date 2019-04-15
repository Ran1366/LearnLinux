#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>

int main()
{
  struct sockaddr_in server_addr;
  int confd, len;
  char ipstr[] = "192.168.197.133";
  char buf[1024];
  //socket
  confd = socket(AF_INET, SOCK_STREAM, 0);

  //set ip address and port 
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(7000);
  inet_pton(AF_INET, ipstr, &server_addr.sin_addr.s_addr);

  //connect with server
  connect(confd, (struct sockaddr*)&server_addr, (socklen_t)sizeof(server_addr));

  //Request the server to process the data
  while(fgets(buf, sizeof(buf), stdin))
  {
    if(strcmp(buf, "eof\n") == 0)
    {
      write(confd, buf, strlen(buf)); 
      printf("退出客户端\n");
      break;
    }
    write(confd, buf, strlen(buf)); 
    len = read(confd, buf, sizeof(buf));
    write(STDOUT_FILENO, buf, len);
  }
  //close the socket
  close(confd);
  return 0;
}
