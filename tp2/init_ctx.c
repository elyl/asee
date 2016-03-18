#include <stdlib.h>
#include <stdio.h>
#include "schedule.h"
#include "hardware.h"

int init_ctx(ctx_t *ctx, int stack_size, func_t *f, void *args)
{
  if ((ctx->ebp = malloc(stack_size)) == NULL)
    {
      fprintf(stderr, "Unable to create a new context\n");
      return (-1);
    }
  ctx->base = ctx->ebp;
  ctx->esp = ctx->ebp + stack_size - sizeof(int);
  ctx->ebp = ctx->esp;
  ctx->f = f;
  ctx->args = args;
  ctx->state = CTX_START;
  irq_enable();
  return (1);
}
