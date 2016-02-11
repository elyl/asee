#ifndef __SCHEDULE_H__
#define __SCHEDULE_H__

#include <time.h>

typedef void (func_t)(void*);

typedef struct ctx_s
{
  void		*ebp;
  void		*esp;
  void		*base;
  func_t	*f;
  void		*args;
  char		state;
  char		name[255];
  time_t	start;
  time_t	current;
  struct ctx_s	*next;
  struct ctx_s	*prev;
}	ctx_t;

extern ctx_t	*ctx;

int	create_ctx(int stack_size, char *name, func_t *f, void *args);
int	init_ctx(ctx_t *ctx, int stack_size, func_t *f, void *args);

void	yield();
void	start_sched();
void	switch_to_ctx(ctx_t *next_ctx);

#endif
