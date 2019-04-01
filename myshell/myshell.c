#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
char *argv[32];
int argc;
char buf[1024] = {0};
int redirect = 0;
char *file = NULL;
void do_face()
{
  printf("[rt123@localhost minishell]$ ");
  fflush(stdout);
  memset(buf, 0x00, 1024);
  if(scanf("%[^\n]", buf) != 1)
  {
    getchar();
  }
}

void do_parse()
{
  char *ptr = buf;
  argc = 0;
  redirect = 0;
  file = NULL;
  while(*ptr != '\0')
  {
    if(*ptr == '>')
    {
      redirect = 1;
      *ptr++ = '\0';
      if(*ptr == '>')
      {
        redirect = 2;
        *ptr++ = '\0';
      }
      while(isspace(*ptr) && *ptr != '\0')
      {
        ptr++;
      }
      file = ptr;
      while(!isspace(*ptr) && *ptr != '\0')
      {
        ptr++;
      }
      *ptr = '\0';
    }
    ptr++;
  }
  ptr = buf;
  while(*ptr != '\0')
  {
    if(!isspace(*ptr))
    {
      argv[argc++] = ptr;
      while(!isspace(*ptr) && *ptr != '\0')
      {
        ptr++;
      }
    }
    else{
      *ptr++ = '\0';
    }
  }
  argv[argc] = NULL;
}
int main()
{
  while(1)  
  {
    fflush(stdout);
    do_face();
    do_parse();
    int pid = fork();
    if(pid < 0)
    {
      perror("fork");
      exit(1);
    }
    else if(pid == 0)
    {
      //at child;
      if(redirect == 1)
      {
        int fd = open(file, O_CREAT | O_TRUNC | O_WRONLY, 0664);
        dup2(fd, STDOUT_FILENO);
      }
      if(redirect == 2)
      {
        int fd = open(file, O_CREAT | O_APPEND | O_WRONLY, 0664);
        dup2(fd, STDOUT_FILENO);
      }
      execvp(argv[0], argv);
      exit(0);
    }
    else{
      wait(NULL);
    }
  }
  return 0;
}
