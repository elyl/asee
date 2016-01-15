#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

static t_command cmd[] = {
  {"ps",	ps},
  {"ls",	ls},
  {"cat",	cat},
  {"compute",	compute}
};

void shell()
{
  char		buffer[BUFFER_SIZE];
  int		c;
  int		i;

  //Lancer ordonanceur
  printf("$>");
  fflush(stdout);
  while ((c = read(0, buffer, BUFFER_SIZE - 1)) != -1)
    {
      buffer[c] = '\0';

      i = 0;
      while (strcmp(, cmd[i].str) != 0 && i < NB_CMD)
	++i;
      if (i == NB_CMD)
	printf("Unknown command\n");
      else
	{
	  if (buffer[c - 1] == '&')
	    {
	      buffer[c - 1] = '\0';
	      //Create ctx
	    }
	  
	  cmd[i].fun(arg);
	}
      printf("$>");
      fflush(stdout);
    }
}
