#include "schedule.h"
#include "hardware.h"

void do_nothing()
{
  return;
}

void start_sched()
{
  int	i;
  
  init_hardware("core.ini");
  i = 0;
  while (i < 16)
    IRQVECTOR[i++] = do_nothing;
  IRQVECTOR[TIMER_IRQ] = time_handler;
  IRQVECTOR[CORE_IRQ] = core_init;
  //_mask(0);
  _out(TIMER_PARAM,128+64+32+8); /* reset + alarm on + 8 tick / alarm */
  _out(TIMER_ALARM,0xFFFFFFFE);  /* alarm at next tick (at 0xFFFFFFFF) */
  _out(CORE_STATUS, 0x02);
}
