#include "schedule.h"
#include "hw.h"
#include "drive.h"

void start_sched()
{
  setup_irq(TIMER_IRQ, yield);
  setup_irq(HDA_IRQ, inst_done);
  start_hw();
}
