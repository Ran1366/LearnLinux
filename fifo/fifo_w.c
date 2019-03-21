#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  char buf[1024] = {"hello world\n"};
  if(argc < 2) 
  {
    printf("./fifo_w fifoname\n");
    exit(1);
  }
  int fd = open(argv[1], O_WRONLY);
  if(fd < 0)
  {
    perror("open");
    exit(1);
  }
  //int flags = fcntl(fd, F_GETFL);
  //flags |= O_NONBLOCK;
  //fcntl(fd, F_SETFL, flags);
  write(fd, buf, strlen(buf));
  close(fd);
  return 0;
}
