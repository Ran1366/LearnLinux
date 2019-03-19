#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
  if(argc < 2)
  {
    printf("./app file\n");
    exit(1);
  }
  int fd = open(argv[1], O_RDONLY);
  if(fd < -1)
  {
    perror("open");
    exit(1);
  }
  dup2(fd, STDIN_FILENO);
  close(fd);
  execl("./upper", "upper", NULL);
  perror("./upper");
  exit(1);
}
