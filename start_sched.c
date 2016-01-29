#include "schedule.h"
#include "hw.h"

void start_sched()
{
  setup_irq(TIMER_IRQ, yield);
  start_hw();
}
