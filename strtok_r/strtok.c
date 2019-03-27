#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{
  char buf[] = "hellow world i'm RT";
  char *p;
  char *save = buf;
  while((p = strtok_r(save," ", &save)))
  {
    printf("%s\n", p);
  }
  return 0;
}
