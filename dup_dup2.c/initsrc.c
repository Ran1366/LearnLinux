#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
  FILE *fd = fopen("src","w");
  int buf[1024] = {0};
  int i = 0;
  srand(time(0));
  for(; i < 1024; ++i)
  {
    buf[i] = rand()%1024;
  }
  for(i = 0; i < 1024; ++i)
  {    
    fprintf(fd, "%d ", buf[i]);
  }
  return 0;
}
