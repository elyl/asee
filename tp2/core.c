#include <stdio.h>
#include "schedule.h"
#include "hardware.h"

void time_handler()
{
  change_ctx_state(_in(CORE_ID));
  _out(TIMER_ALARM,0xFFFFFFFE);
  yield();
}

void core_init()
{
  printf("initialisation core %d\n", _in(CORE_ID));
  _out (CORE_IRQMAPPER + _in (CORE_ID), 0x1 << TIMER_IRQ);
  _mask(TIMER_IRQ);
  while (1)
    ;
}

void irq_disable()
{
  _mask(16);
}

void irq_enable()
{
  _mask(1);
}
