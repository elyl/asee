#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "shell.h"

void ps(void *arg)
{
  printf("ps NYI\n");
  return;
}

void ls(void *arg)
{
  printf("ls NYI\n");
  return;
}

void cat(void *arg)
{
  printf("cat NYI\n");
  return;
}

// Faire un parsing plus propre, plante si pas d'arg
void compute(void *arg)
{
  char	*ptr;

  strtok((char*)arg, " ");
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
   
void quit(void *arg)
{
  exit(0);
}
