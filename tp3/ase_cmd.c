#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/init.h>
#include <linux/seq_file.h>
#include <linux/jiffies.h>
#include <linux/string.h>
#include <asm/uaccess.h>

static int
ase_cmd_proc_open(struct inode *inode, struct file *file)
{
  printk(KERN_INFO "ASE_CMD: Open has been called");
  return 0;
}

static ssize_t
ase_cmd_proc_write(struct file *filp, const char __user *buff,
		   size_t len, loff_t *data)
{
  printk(KERN_INFO "ASE_CMD: Wirte has been called");
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
