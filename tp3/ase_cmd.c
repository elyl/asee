#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/init.h>
#include <linux/seq_file.h>
#include <linux/jiffies.h>
#include <linux/string.h>
#include <linux/pid.h>
#include <linux/sched.h>
#include <linux/cputime.h>
#include <asm/uaccess.h>

#define ASE_CMD_BUFFER_LEN 6
static char		ase_cmd_buffer[ASE_CMD_BUFFER_LEN];

#define ASE_CMD_MAX_PID 10
static struct pid	*ase_cmd_pid[ASE_CMD_MAX_PID] = {NULL};
static int		ase_cmd_pid_total = 0;

static struct proc_dir_entry	*ase_proc_parent;

static int
ase_cmd_proc_show(struct seq_file *m, void *v)
{
  static struct task_struct	*task;
  int				n;

  n = 0;
  task = pid_task(ase_cmd_pid[n], PIDTYPE_PID);
  if (task != NULL)
    {
      seq_printf(m, "Je suis une loutre ! %d",
		 cputime_to_usecs(task->utime + task->stime));
    }
 else
    {
      ase_cmd_pid[n] = NULL;
      ase_cmd_pid_total = 0;
      printk(KERN_INFO "ASE_CMD: Task doesn't exist anymore");
      return -1;
    }
  return 0;
}

static int
ase_cmd_proc_open(struct inode *inode, struct file *file)
{
  return single_open(file, ase_cmd_proc_show, NULL);
}

static ssize_t
ase_cmd_proc_write(struct file *filp, const char __user *buff,
		   size_t len, loff_t *data)
{
  long		res;
  struct pid	*tmp_pid;
  int		i;
  
  printk(KERN_INFO "ASE_CMD: Write has been called");
  if (len > (ASE_CMD_BUFFER_LEN - 1)) {
    printk(KERN_INFO "ASE_CMD: error, input too long");
    return -EINVAL;
  }
  else if (copy_from_user(ase_cmd_buffer, buff, len)) {
    return -2;
  }
  ase_cmd_buffer[len] = 0;

  kstrtol(ase_cmd_buffer, 0, &res);
  
  rcu_read_lock();
  tmp_pid = find_vpid((int)res);
  rcu_read_unlock();

  if (!tmp_pid)
    {
      printk(KERN_INFO "ASE_CMD: PID not found");
      return -1;
    }
  if (ase_cmd_pid_total >= ASE_CMD_MAX_PID)
    {
      printk(KERN_INFO "ASE_CMD: Max PID reached");
      return -3;
    }
  ++ase_cmd_pid_total;

  for (i = 0;ase_cmd_pid[i] != NULL;++i)
    ;
  ase_cmd_pid[i] = tmp_pid;

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
  printk(KERN_INFO "ASE_CMD: Coucou 1");
  ase_proc_parent = proc_mkdir("ase", NULL);
    if (!ase_proc_parent)
    {
      printk(KERN_INFO "ASE_CMD: Error creating the directory /proc/ase");
      return -ENOMEM;
    }
    printk(KERN_INFO "ASE_CMD: Coucou 2");
  if (!proc_create("ase_cmd", 0666, NULL, &ase_cmd_proc_fops))
    {
      printk(KERN_INFO "ASE_CMD: Error creating the file /proc/ase_cmd");
      return -ENOMEM;
    }
  return 0;
}

static void __exit
ase_cmd_proc_exit(void)
{
  remove_proc_entry("ase_cmd", NULL);
  remove_proc_entry("ase", NULL);
}

module_init(ase_cmd_proc_init);
module_exit(ase_cmd_proc_exit);

MODULE_LICENSE("GPL");
