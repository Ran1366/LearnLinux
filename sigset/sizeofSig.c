#include <stdio.h>
#include <signal.h>

int main()
{
  printf("%lu\n", sizeof(sigset_t));
  return 0;
}
