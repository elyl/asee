#include <stdio.h>
#include <string.h>
#include "hardware.h"
#include "hw.h"
#include "drive.h"
#include "schedule.h"

static t_inst	inst[255];
static int	current = 0;
static int	max = 0;

void add_inst(unsigned char *param, unsigned char cmd, char *buffer)
{
  int	num;
  
  inst[num].param = param;
  inst[num].buffer = buffer;
  inst[num].done = 0;
  inst[num].cmd = cmd;
  num = max;
  ++max;
  if (max > 255)
    max = 0;
  run_inst(num);
}

void run_inst(int id)
{
  printf("Waiting for disk to be done...\n");
  while (current != max)
    yield();
  printf("Disk done !\n");
  if (inst[id].cmd == CMD_WRITE)
    memcpy(MASTERBUFFER, inst[id].buffer, disk.sector_size);
  _out(HDA_PARAMREG, inst[id].param[0]);
  _out(HDA_PARAMREG + 1, inst[id].param[1]);
  _out(HDA_PARAMREG + 2, inst[id].param[2]);
  _out(HDA_PARAMREG + 3, inst[id].param[3]);
  _out(HDA_PARAMREG + 4, inst[id].param[4]);
  _out(HDA_PARAMREG + 5, inst[id].param[5]);
  _out(HDA_CMDREG, inst[id].cmd);
  printf("Waiting for cmd to be done..\n");
  while (inst[id].done == 0)
    yield();
  if (inst[id].cmd == CMD_READ)
    memcpy(inst[id].buffer, MASTERBUFFER, disk.sector_size);
}

void inst_done()
{
  inst[current].done = 1;
  current = (current == 255) ? 0 : current + 1;
  printf("Disk interaction done !\n");
}
