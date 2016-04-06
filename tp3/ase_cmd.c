#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/init.h>
#include <linux/seq_file.h>
#include <linux/jiffies.h>
#include <linux/string.h>
#include <linux/pid.h>
#include <asm/uaccess.h>

#define ASE_CMD_BUFFER_LEN	6
static char		ase_cmd_buffer[ASE_CMD_BUFFER_LEN];
static struct pid	*ase_cmd_pid;

static int
ase_cmd_proc_open(struct inode *inode, struct file *file)
{
  printk(KERN_INFO "ASE_CMD: Open has been called");
  return 0;
}

static void
ase_cmd_new_pid(long res)
{
  int	pid;

  pid = res;
  rcu_read_lock();
  ase_cmd_pid = find_vpid(pid);
  rcu_read_unlock();
}

static ssize_t
ase_cmd_proc_write(struct file *filp, const char __user *buff,
		   size_t len, loff_t *data)
{
  long	res;
  
  printk(KERN_INFO "ASE_CMD: Wirte has been called");
  if (len > (ASE_CMD_BUFFER_LEN - 1)) {
    printk(KERN_INFO "ASE_CMD: error, input too long");
    return -EINVAL;
  }
  else if (copy_from_user(ase_cmd_buffer, buff, len))
    return -2;
  ase_cmd_buffer[len] = 0;

  kstrtol(ase_cmd_buffer, 0, &res);
  ase_cmd_new_pid(res);
  return len;
}

static const struct file_operations ase_cmd_proc_fops = {
  .owner      = THIS_MODULE,
  .open       = ase_cmd_proc_open,
  .read       = seq_read,
  .write      = ase_cmd_proc_write,
  .llseek     = seq_lseek,
  .release    = single_release,
};

static int __init
ase_cmd_proc_init(void)
{
  proc_create("ase_cmd", 0666, NULL, &ase_cmd_proc_fops);
  return 0;
}

static void __exit
ase_cmd_proc_exit(void)
{
  remove_proc_entry("ase_cmd", NULL);
}

module_init(ase_cmd_proc_init);
module_exit(ase_cmd_proc_exit);
