#include <stdio.h>
#include <unistd.h>

int main()
{
  int pid = fork();
  if(pid > 0)
  {
    printf("i'm parent\n");
  }
  if(pid == 0)
  {
    printf("i'm child\n");
    execlp("ls", "ls", "-l", NULL);
  }
  return 0;
}
