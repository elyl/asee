#include <stdlib.h>
#include <stdio.h>
#include "schedule.h"
#include "hw.h"

static ctx_t	*ctx;
static ctx_t	ctx_main;

int create_ctx(int stack_size, func_t *f, void *args)
{
  ctx_t	*ptr;

  irq_disable();
  if ((ptr = malloc(sizeof(ctx_t))) == NULL)
    {
      fprintf(stderr, "Unable to create a new context\n");
      return (-1);
    }
  if (ctx == NULL)
    ctx = ptr;
  else
    {
      ptr->next = ctx->next;
      ctx->next = ptr;
    }
  ctx->next = ptr;
  ptr->next->prev = ptr;
  ptr->prev = ctx;
  return (init_ctx(ptr, stack_size, f, args));
}

void yield()
{
  ctx_t	*ptr;

  printf("Changement de contexte !\n");
  irq_disable();
  if (ctx->state == 1 && ctx->prev->state == 1)
    {
      ctx_main.state = 0;
      asm ("movl %%esp, %0"
	   :"=r"(ctx_main.esp)
	   :);
      asm ("movl %%ebp, %0"
	   :"=r"(ctx_main.ebp)
	   :);
    }
  else
    {
      asm ("movl %%esp, %0"
	   :"=r"(ctx->esp)
	   :);
      asm ("movl %%ebp, %0"
	   :"=r"(ctx->ebp)
	   :);
    }
  ctx = ctx->next;
  if (ctx->state == 2)
    {
      printf("fin de contexte\n");
      ctx->prev->next = ctx->next;
      ctx->next->prev = ctx->prev;
      ptr = ctx;
      ctx = ctx->next;
      free(ptr->base);
      free(ptr);
    }
  if (ctx == ptr)
    ctx = &ctx_main;
  asm ("movl %0, %%esp"
       :
       :"r"(ctx->esp));
  asm ("movl %0, %%ebp"
       :
       :"r"(ctx->ebp));
  irq_enable();
  if (ctx->state == 1)
    {
      ctx->state = 0;
      ctx->f(ctx->args);
      ctx->state = 2;
      yield();
    }
}
