#include <stdio.h>
#include "hardware.h"
#include "hw.h"

void do_nothing()
{
  return;
}

void _seek(unsigned int cylinder, unsigned int sector)
{
  _out(HDA_PARAMREG, (cylinder >> 8) & 0xff);
  _out(HDA_PARAMREG + 1, cylinder & 0xff);
  _out(HDA_PARAMREG + 2, (sector >> 8) & 0xff);
  _out(HDA_PARAMREG + 3, sector & 0xff);
  _out(HDA_CMDREG, CMD_SEEK);
  _sleep(HDA_IRQ);
}

int frmt()
{
  int	cylinders;
  int	sectors;
  int	i;
  int	j;

  if (init_hardware("hardware.ini") == 0)
    {
      fprintf(stderr, "Error in hardware initialization\n");
      return (-1);
    }
  for (i = 0; i < 16; ++i)
    IRQVECTOR[i] = do_nothing;
  _mask(1);
  _out(HDA_PARAMREG, 0);
  _out(HDA_PARAMREG + 1, 0);
  _out(HDA_PARAMREG + 2, 0);
  _out(HDA_PARAMREG + 3, 0);
  _out(HDA_PARAMREG + 4, 0);
  _out(HDA_PARAMREG + 5, 0);
  _out(HDA_CMDREG, CMD_DSKINFO);
  cylinders = _in(HDA_PARAMREG) << 8;
  cylinders |= _in(HDA_PARAMREG + 1);
  sectors = _in(HDA_PARAMREG + 2) << 8;
  sectors |= _in(HDA_PARAMREG + 3);
  for (i = 0; i < cylinders; ++i)
    {
      for (j = 0; j < sectors; ++j)
	{
	  _seek(i, j);
	  _out(HDA_PARAMREG, 0);
	  _out(HDA_PARAMREG + 1, 1);
	  _out(HDA_PARAMREG + 2, 0);
	  _out(HDA_PARAMREG + 3, 0);
	  _out(HDA_PARAMREG + 4, 0);
	  _out(HDA_PARAMREG + 5, 0);
	  _out(HDA_CMDREG, CMD_FORMAT);
	  _sleep(HDA_IRQ);
	}
    }
  return (0);
}

int main(int argc, char **argv)
{
  argc = argc;
  argv = argv;
  frmt();
  return (0);
}
