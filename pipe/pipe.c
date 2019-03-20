#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
  int fd[2];
  char buf[] = "hello world\n";
  char str[1024];
  if(pipe(fd) == -1)
  {
    perror("pipe");
    exit(1);
  }
  int pid = fork();
  if(pid > 0)
  {
    //in parent close read of parent
    close(fd[0]);
    write(fd[1], buf, strlen(buf));
    wait(NULL);
  }
  if(pid == 0)
  {
    //in child close write of child
    close(fd[1]);
    int len = read(fd[0], str, sizeof(str));
    write(STDOUT_FILENO, str, len);
  }
  if(pid < 0) 
  {
    perror("fork");
    exit(1);
  }
  return 0;
}
