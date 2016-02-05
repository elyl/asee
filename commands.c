#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "shell.h"

void ps(char *arg)
{
  printf("ps NYI\n");
  return;
}

void ls(char *arg)
{
  printf("ls NYI\n");
  return;
}

void cat(char *arg)
{
  printf("cat NYI\n");
  return;
}

void compute(char *arg)
{
  char	*ptr;

  strtok(arg, " ");
  ptr = strtok(NULL, " ");
  if (strlen(ptr) != 0)
    printf("Fibo %d : %d\n", atoi(ptr), do_fibo(atoi(ptr)));
  else
    printf("Parse error\n");
}

int do_fibo(int n)
{
  if (n == 0)
    return (0);
  if (n == 1)
    return (1);
  if (n == 2)
    return (2);
  else
    return (do_fibo(n - 1) + do_fibo(n - 2));
}
   
void quit(char *arg)
{
  exit(0);
}
