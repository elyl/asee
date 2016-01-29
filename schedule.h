#ifndef __SCHEDULE_H__
#define __SCHEDULE_H__

typedef void (func_t)(void*);

typedef struct ctx_s
{
  void		*ebp;
  void		*esp;
  void		*base;
  func_t	*f;
  void		*args;
  char		state;
  struct ctx_s	*next;
  struct ctx_s	*prev;
}	ctx_t;

int	create_ctx(int stack_size, func_t *f, void *args);
int	init_ctx(ctx_t *ctx, int stack_size, func_t *f, void *args);

void	yield();
void	start_sched();
void	switch_to_ctx(ctx_t *next_ctx);

#endif
