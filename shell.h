#ifndef __SHELL_H__
#define __SHELL_H__

#define BUFFER_SIZE	255
#define NB_CMD		4

typedef struct s_command
{
  char	*str;
  void (*fun) (char*);
}	t_command;

void	ps(char *arg);
void	ls(char *arg);
void	cat(char *arg);
void	compute(char *arg);
void	shell();

int	do_fibo(int n);

#endif
