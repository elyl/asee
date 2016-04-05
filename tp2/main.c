#include <stdlib.h>
#include <stdio.h>
#include "schedule.h"

void f_ping(void *arg)
{
  //while (1)
    printf("ping ping ping\n");
}

void f_pong(void *arg)
{
  //while (1)
    printf("pong pong pong\n");
    while (1)
      ;
}

int main(int argc, char **argv)
{
  argc = argc;
  argv = argv;
  create_ctx(2048, "tutu", f_ping, NULL);
  create_ctx(2048, "tutu", f_pong, NULL);
  start_sched();
  //yield();
  while (1)
    ;
  //printf("maaaaaaaaaaaaaaaaaaaaaain\n");
  return (0);
}
