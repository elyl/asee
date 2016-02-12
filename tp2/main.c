#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "hardware.h"

void init()
{
  unsigned char	nb;

  nb = _in(CORE_ID);
  printf("Core : %d\n", nb);
}

int main()
{
  printf("%d\n", init_hardware("core.ini"));
  IRQVECTOR[0] = init;
  _out(CORE_STATUS, 0xff);
  sleep(20);
  return (0);
}
