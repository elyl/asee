#include <stdio.h>
#include <string.h>
#include "hardware.h"
#include "hw.h"
#include "drive.h"

t_disk disk = {0, 0, 0};

void do_nothing()
{
  return;
}

void copy (unsigned char *dest, const unsigned char *src)
{
  int	i;
  int	sector_size;

  _out(HDA_CMDREG, CMD_DSKINFO);
  sector_size = _in(HDA_PARAMREG + 4) << 8;
  sector_size |= _in(HDA_PARAMREG + 5);
  i = 0;
  while (i < sector_size)
    {
      dest[i] = src[i];
      ++i;
    }
}

void init_drive()
{
  int	i;

  if (init_hardware("hardware.ini") == 0)
    fprintf(stderr, "Error in hardware initialization\n");
  i = 0;
  while (i < 16)
    IRQVECTOR[i++] = do_nothing;
  _mask(1);
  disk_info();
}

void disk_info()
{
  _out(HDA_CMDREG, CMD_DSKINFO);
  disk.nb_cylinders = _in(HDA_PARAMREG) << 8;
  disk.nb_cylinders |= _in(HDA_PARAMREG + 1);
  disk.nb_sectors = _in(HDA_PARAMREG + 2) << 8;
  disk.nb_sectors |= _in(HDA_PARAMREG + 3);
  disk.sector_size = _in(HDA_PARAMREG + 4) << 8;
  disk.sector_size |= _in(HDA_PARAMREG + 5);  
}

void seek(unsigned int cylinder, unsigned int sector)
{
  unsigned char	param[6];

  param[0] = (cylinder >> 8) & 0xff;
  param[1] = cylinder & 0xff;
  param[2] = (sector >> 8) & 0xff;
  param[3] = sector & 0xff;
  add_inst(param, CMD_SEEK, NULL);
  /*_out(HDA_PARAMREG, (cylinder >> 8) & 0xff);
  _out(HDA_PARAMREG + 1, cylinder & 0xff);
  _out(HDA_PARAMREG + 2, (sector >> 8) & 0xff);
  _out(HDA_PARAMREG + 3, sector & 0xff);
  _out(HDA_CMDREG, CMD_SEEK);
  _sleep(HDA_IRQ);*/
}

void read_sector(unsigned int cylinder, unsigned int sector, unsigned char *buffer)
{  
  seek(cylinder, sector);
  _out(HDA_PARAMREG, 0);
  _out(HDA_PARAMREG, 1);
  _out(HDA_CMDREG, CMD_READ);
  _sleep(HDA_IRQ);
  memcpy(buffer, MASTERBUFFER, disk.sector_size);
}

void write_sector(unsigned int cylinder, unsigned int sector, const unsigned char *buffer)
{
  seek(cylinder, sector);
  memcpy(MASTERBUFFER, buffer, disk.sector_size);
  _out(HDA_PARAMREG, 0);
  _out(HDA_PARAMREG, 1);
  _out(HDA_CMDREG, CMD_WRITE);
  _sleep(HDA_IRQ);
}

void format_sector(unsigned int cylinder, unsigned int sector, unsigned int nsector, unsigned int value)
{
  seek(cylinder, sector);
  _out(HDA_PARAMREG, (nsector >> 8) & 0xff);
  _out(HDA_PARAMREG + 1, nsector & 0xff);
  _out(HDA_PARAMREG + 2, (value >> 24) & 0xff);
  _out(HDA_PARAMREG + 3, (value >> 16) & 0xff);
  _out(HDA_PARAMREG + 4, (value >> 8) & 0xff);
  _out(HDA_PARAMREG + 5, value & 0xff);
  _out(HDA_CMDREG, CMD_FORMAT);
  _sleep(HDA_IRQ);
}
