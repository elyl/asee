#ifndef __DRIVE_H__
#define __DRIVE_H__

typedef struct s_disk
{
  unsigned int	nb_cylinders;
  unsigned int	nb_sectors;
  unsigned int	sector_size;
}	t_disk;

typedef struct s_inst
{
  unsigned char		*param;
  unsigned char		cmd;
  char			done;
  char			*buffer;
}	t_inst;

extern t_disk	disk;

void	disk_info();
void	seek(unsigned int cylinder, unsigned int sector);
void	copy(unsigned char *dest, const unsigned char *src);
void	init_drive();
void	do_nothing();
void	read_sector(unsigned int cylinder, unsigned int sector, unsigned char *buffer);
void	write_sector(unsigned int cylinder, unsigned int sector, const unsigned char *buffer);
void	format_sector(unsigned int cylinder, unsigned int sector, unsigned int nsector, unsigned int value);

void	add_inst(unsigned char *inst, unsigned char cmd, char *buffer);
void	run_inst(int id);
void	inst_done();

#endif
