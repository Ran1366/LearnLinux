#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>

#define MAXLEN 0x1000 
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
  int fd = open(argv[1], O_RDWR | O_CREAT, 0777);
  if(fd < 0) 
  {
    sys_err("open", 1);
  }
  //如果创建了一个文件,该文件需扩容才能使用
  lseek(fd, MAXLEN - 1, SEEK_SET);
  write(fd, "\0", 1);

  void *p = mmap(NULL, MAXLEN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if(p == MAP_FAILED)
  {
    sys_err("mmap", 1);
  }
  close(fd); 


  char *pp = (char *)p;
  int i = 0;
  while(1)
  {
    sprintf(pp, "hello world%d\n", i++);
    sleep(1);
  }
  munmap(p, MAXLEN);
  return 0; 
}
