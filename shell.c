#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

static t_command cmd[] = {
  {"ps",	ps},
  {"ls",	ls},
  {"cat",	cat},
  {"compute",	compute},
  {"exit",	quit}
};

void shell()
{
  char		buffer[BUFFER_SIZE + 1];
  int		c;
  int		i;

  //Lancer ordonanceur
  printf("$>");
  fflush(stdout);
  while ((c = read(0, buffer, BUFFER_SIZE)) != -1)
    {
      buffer[c] = '\0';
      i = 0;
      printf("prout\n");
      while (c >= (int)strlen(cmd[i].str) && strncmp(buffer, cmd[i].str, strlen(cmd[i].str)) != 0 && i < NB_CMD)
	{
	  printf("Ca boucle\n");
	  ++i;
	}
      if (i == NB_CMD)
	printf("Unknown command\n");
      else
	{
	  if (buffer[c - 1] == '&')
	    {
	      buffer[c - 1] = '\0';
	      //Create ctx
	    }	  
	  cmd[i].fun(buffer);
	}
      printf("$>");
      fflush(stdout);
    }
}
