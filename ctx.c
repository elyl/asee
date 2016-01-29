#include <stdio.h>
#include <stdlib.h>
#include "schedule.h"

void f_ping(void *arg);
void f_pong(void *arg);
void f_pang(void *arg);
void f_pung(void *arg);

int main()
{
  start_sched();
  create_ctx(16384, f_ping, NULL);
  create_ctx(16384, f_pong, NULL);
  //create_ctx(16384, f_pang, NULL);
  yield();

  //yield();
  exit(EXIT_SUCCESS);
  return (1);
}

void f_ping(void *args)
{
  int	i;

  i = 0;
  args = args;
  printf("ping\n");
  while(10)
    {
      printf("A\n") ;
      //yield();
      printf("B\n") ;
      //yield();
      printf("C\n") ;
      //yield();
      ++i;
  }
}

void f_pong(void *args)
{
  int	i;
  
  args = args;
  i = 0;
  printf("pong\n");
  while(10)
    {
      printf("1\n") ;
      //yield();
      printf("2\n") ;
      //yield();
      ++i;
  }
}

void f_pang(void *args)
{
  int	i;

  //create_ctx(16385, f_pung, NULL);
  i = 0;
  args = args;
  while (10)
    {
      printf("+");
      yield();
      printf("-");
      yield();
      ++i;
    }
}

void f_pung(void *args)
{
  args = args;
  while (1)
    {
      printf("$");
      yield();
      printf("€");
      yield();
      printf("£");
      yield();
    }
}
