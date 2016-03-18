#ifndef __SHELL_H__
#define __SHELL_H__

#define BUFFER_SIZE	255
#define NB_CMD		5

typedef struct s_command
{
  char	*str;
  void (*fun) (void*);
}	t_command;

void	ps(void *arg);
void	ls(void *arg);
void	cat(void *arg);
void	compute(void *arg);
void	quit(void *arg);
void	shell(void *arg);

int	do_fibo(int n);

#endif
