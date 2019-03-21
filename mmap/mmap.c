#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

int main()
{
  void *p;
  int fd = open("hello", O_RDWR);
  if(fd < 0)
  {
    perror("open");
    exit(1);
  }
  int len = lseek(fd, 0, SEEK_END);
  p = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if(p == MAP_FAILED)
  {
    perror("mmap");
    exit(1);
  }
  int *pp = (int *)p;
  pp[0] = 0x30313233;
  munmap(p, len);
  return 0;
}
