#ifndef __SEM_H__
#define __SEM_H__

typedef struct sem_s
{
  int	counter;
}	sem_t;

void	init_ctx(sem_t *sem, int n);
void	sem_down(sem_t *sem);
void	sem_up(sem_t *sem);

#endif
