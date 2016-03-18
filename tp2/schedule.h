#ifndef __SCHEDULE_H__
#define __SCHEDULE_H__

#define TIMER_IRQ	2
#define TIMER_ALARM	0xF8
#define TIMER_PARAM	0xF4
#define TIMER_CLOCK	0xF0
#define NB_CORE		8
#define CORE_IRQ	0
#define CORE_STATUS	0x80
#define CORE_IRQMAPPER	0x82
#define CORE_ID		0x126

#include <time.h>

typedef void (func_t)(void*);

enum ctx_state{CTX_START, CTX_RUN, CTX_STOP, CTX_END};

typedef struct ctx_s
{
  void			*ebp;
  void			*esp;
  void			*base;
  func_t		*f;
  void			*args;
  enum ctx_state	state;
  char			name[255];
  struct ctx_s		*next;
  struct ctx_s		*prev;
}	ctx_t;

//extern ctx_t	*ctx;

ctx_t	*find_next_iddle();

int	create_ctx(int stack_size, char *name, func_t *f, void *args);
int	init_ctx(ctx_t *ctx, int stack_size, func_t *f, void *args);

void	time_handler();
void	core_init();
void	yield();
void	start_sched();
void	switch_to_ctx(ctx_t *next_ctx, int core);
void	irq_enable();
void	irq_disable();
void	clean_ctx(ctx_t *ptr);
void	change_ctx_state(int core);

#endif
