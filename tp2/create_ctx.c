#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "schedule.h"
#include "hardware.h"

ctx_t		*ctx;
static ctx_t	ctx_main[NB_CORE];
static ctx_t	*ctx_current[NB_CORE];
int		core;

int create_ctx(int stack_size, char *name, func_t *f, void *args)
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
  strcpy(ptr->name, name);
  return (init_ctx(ptr, stack_size, f, args));
}

void change_ctx_state(int core)
{
  if (ctx_current[core] != NULL)
    {
      ctx_current[core]->state = CTX_STOP;
    }
}

// Removes the context ptr from the contexts lists
void clean_ctx(ctx_t *ptr)
{
  if (ctx == ptr)
    ctx = ctx->next;
  ptr->prev->next = ptr->next;
  ptr->next->prev = ptr->prev;
  if (ptr->next == ptr)
    ctx = NULL;
  free(ptr->base);
  free(ptr);
}

// Search for the first context which is either paused or te be started
ctx_t	*find_next_iddle()
{
  ctx_t	*ptr;

  ptr = ctx;
  while (ptr->next != ctx)
    {
      if (ptr->state == CTX_STOP || ptr->state == CTX_START)
	return (ptr);
      ptr = ptr->next;
    }
  ctx = ptr;
  return ((ptr->state == CTX_STOP || ptr->state == CTX_START) ? ptr : NULL);
}

// Change current context to new_ctx on core "core"
void switch_to_ctx(ctx_t *new_ctx, int core)
{
  // Sauvegarde du contexte d'execution courant    
  asm ("movl %%esp, %0"
       :"=r"(ctx_main[core].esp)
       :);
  asm ("movl %%ebp, %0"
       :"=r"(ctx_main[core].ebp)
       :);
  ctx_current[core] = new_ctx;
  //Installation du nouveau contexte
  asm ("movl %0, %%esp"
       :
       :"r"(new_ctx->esp));
  asm ("movl %0, %%ebp"
       :
       :"r"(new_ctx->ebp));
  //CORE UNLOCK
  irq_enable();
  // Si premier lancement on appelle f()
  if (ctx_current[_in(CORE_ID)]->state == CTX_START)
    {
      ctx_current[_in(CORE_ID)]->state = CTX_RUN;
      ctx_current[_in(CORE_ID)]->f(ctx_current[_in(CORE_ID)]->args);
    }
  else // Sinon on se contente de noter le contexte comme étant en cours d'éxécution
      {
      ctx_current[_in(CORE_ID)]->state = CTX_RUN;
      return;
    }
  irq_disable();
  // Si le contexte a terminé son éxécution on restaure le contexte d'origine
  //CORE LOCK
  asm ("movl %0, %%esp"
	:
	:"r"(ctx_main[_in(CORE_ID)].esp));
  asm ("movl %0, %%ebp"
       :
	:"r"(ctx_main[_in(CORE_ID)].ebp));
  return;
}

void yield()
{
  ctx_t	*current;
  
  if (ctx == NULL)
    return;
  irq_disable();
  //CORE LOCK
  current = find_next_iddle();
  if (current == NULL)
    {
      //CORE UNLOCK
      irq_enable();
      return;
    }
  switch_to_ctx(current, _in(CORE_ID));
  clean_ctx(current);
  //CORE UNLOCK
  irq_enable();
}

/*void yield()
{
  ctx_t		*ptr;

  core = _in(CORE_ID);
  if (ctx[core] == NULL)
    return;
  printf("Changement de contexte !\n");
  irq_disable();
  if (ctx[core]->state == 1 && ctx[core]->prev->state == 1)
    {
      ctx_main[core].state = 0;
      asm ("movl %%esp, %0"
	   :"=r"(ctx_main[core].esp)
	   :);
      asm ("movl %%ebp, %0"
	   :"=r"(ctx_main[core].ebp)
	   :);
    }
  else
    {
      asm ("movl %%esp, %0"
	   :"=r"(ctx[core]->esp)
	   :);
      asm ("movl %%ebp, %0"
	   :"=r"(ctx[core]->ebp)
	   :);
    }
  printf("switch esp, ebp ok\n");
  ctx[core] = ctx[core]->next;
  if (ctx[core]->state == 2)
    {
      printf("fin de contexte\n");
      ctx[core]->prev->next = ctx[core]->next;
      ctx[core]->next->prev = ctx[core]->prev;
      ptr = ctx[core];
      ctx[core] = ctx[core]->next;
      free(ptr->base);
      free(ptr);
    }
  if (ctx[core] == ptr)
    ctx[core] = &ctx_main[core];
  printf("switch2\n");
  asm ("movl %0, %%esp"
       :
       :"r"(ctx[core]->esp));
  asm ("movl %0, %%ebp"
       :
       :"r"(ctx[core]->ebp));
  irq_enable();
  printf("switch2 ok %d\n", ctx[core]->state);
  if (ctx[core]->state == 1)
    {
      printf("Changement de state...\n");
      ctx[core]->state = 0;
      printf("Lancement fonction\n");
      ctx[core]->f(ctx[core]->args);
      printf("Retour de fonction\n");
      ctx[core]->state = 2;
      printf("yield\n");
      yield();
    }
  printf("toto\n");
}
*/
