#include <stdlib.h>
#include "schedule.h"
#include "shell.h"

int main(int argc, char **argv)
{
  argc = argc;
  argv = argv;
  create_ctx(16384, shell, NULL);
  start_sched();
  yield();
  return (0);
}
