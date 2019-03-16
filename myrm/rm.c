#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  if(argc < 2)
  {
    printf("./rm file\n");
    exit(1);
  }
  unlink(argv[1]);
}
