#include <stdio.h>
#include <ctype.h>

int main()
{
  int i;
  while((i = getchar()) != EOF)
  {
    putchar(toupper(i));
  }
  return 0;
}
