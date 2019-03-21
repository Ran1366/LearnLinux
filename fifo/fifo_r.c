#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  if(argc < 2)
  {
    printf("./fifo_r fifoname\n");  
    exit(1);
  }
  int fd = open(argv[1], O_RDONLY);
  if(fd < 0)
  {
    perror("open");
    exit(1);
  }
  char buf[1024];
  int len = read(fd, buf, sizeof(buf));
  //int flags = fcntl(fd, F_GETFL);
  //flags |= O_NONBLOCK;
  //fcntl(fd, F_SETFL, flags);
  write(STDOUT_FILENO, buf, len);
  close(fd);
  return 0;
}
