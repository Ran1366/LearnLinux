#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>

void sys_err(char *str, int exitno)
{
  perror(str);
  exit(exitno);
}
int main(int argc, char *argv[])
{
  if(argc < 2)
  {
    printf("./mmap_w filename\n");
    exit(1);
  }
  int fd = open(argv[1], O_RDWR);
  if(fd < 0) 
  {
    sys_err("open", 1);
  }
  int len = lseek(fd, 0, SEEK_END);
  void *p = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if(p == MAP_FAILED)
  {
    sys_err("mmap", 1);
  }
  close(fd); 


  char *pp = (char *)p;
  while(1)
  {
    printf("%s", pp);
    sleep(1);
  }
  munmap(p, len);
  return 0; 
}
