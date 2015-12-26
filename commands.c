#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "shell.h"

void ps(char *arg)
{
  return;
}

void ls(char *arg)
{
  return;
}

void cat(char *arg)
{
  return;
}

void compute(char *arg)
{
  char	*ptr;

  strtok(arg, " ");
  ptr = strtok(NULL, " ");
  printf("Fibo %d : %d\n", atoi(ptr), do_fibo(atoi(ptr)));
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
   
   
