#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "schedule.h"
#include "shell.h"

static t_command cmd[] = {
  {"ps",	ps},
  {"ls",	ls},
  {"cat",	cat},
  {"compute",	compute},
  {"exit",	quit}
};

void shell(void *arg)
{
  char		buffer[BUFFER_SIZE + 1];
  int		c;
  int		i;

  arg = arg;
  printf("$>");
  fflush(stdout);
  while ((c = read(0, buffer, BUFFER_SIZE)) != -1)
    {
      if (c > 0 && buffer[c - 1] == '\n')
	--c;
      buffer[c] = '\0';
      i = 0;
      while (i < NB_CMD && strncmp(buffer, cmd[i].str, strlen(cmd[i].str)) != 0)
	++i;
      if (i == NB_CMD)
	printf("Unknown command\n");
      else
	{
	  if (buffer[c - 1] == '&')
	    {
	      printf("new context\n");
	      buffer[c - 1] = '\0';
	      create_ctx(16384, cmd[i].str, cmd[i].fun, buffer);
	    }
	  else
	    cmd[i].fun(buffer);
	}
      printf("$>");
      fflush(stdout);
    }
}
