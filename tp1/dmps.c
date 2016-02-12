#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"
#include "hw.h"

static void do_nothing()
{
  return;
}

void _seek(int cylinder, int sector)
{
  _out(HDA_PARAMREG, (cylinder >> 8) & 0xff);
  _out(HDA_PARAMREG + 1, cylinder & 0xff);
  _out(HDA_PARAMREG + 2, (sector >> 8) & 0xff);
  _out(HDA_PARAMREG + 3, sector & 0xff);
  _out(HDA_CMDREG, CMD_SEEK);
}

int dmps(int cylinder, int sector)
{
  int	i;

    if (init_hardware("hardware.ini") == 0)
    {
      fprintf(stderr, "Error in hardware initialization\n");
      return (-1);
    }
  for (i = 0; i < 16; ++i)
    IRQVECTOR[i] = do_nothing;
  _mask(1);
  _seek(cylinder, sector);
  _sleep(HDA_IRQ);
  _out(HDA_PARAMREG, 0);
  _out(HDA_PARAMREG + 1, 1);
  _out(HDA_CMDREG, CMD_READ);
  _sleep(HDA_IRQ);
  for (i = 0; i < HDA_SECTORSIZE; ++i)
    printf("%x ", MASTERBUFFER[i]);
  printf("\n");
  return (0);
}

int main(int argc, char **argv)
{
  if (argc > 2)
    dmps(atoi(argv[1]), atoi(argv[2]));
  else
    fprintf(stderr, "Usage : dmps <cylinder> <sector>\n");
  return (0);
}
