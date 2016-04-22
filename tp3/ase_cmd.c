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
static struct pid	*ase_cmd_pid_struct[ASE_CMD_MAX_PID] = {NULL};
static int		ase_cmd_pid_list[ASE_CMD_MAX_PID] = {0};

static int		ase_cmd_pid_total = 0;

static struct proc_dir_entry	*ase_proc_parent;

static int
ase_cmd_proc_show(struct seq_file *m, void *v)
{
  int	i;

  if (ase_cmd_pid_total == 0)
    {
      seq_printf(m, "You are not monitoring any PID right now");
      printk(KERN_INFO "ASE_CMD: LOUTRE LOUTRE LOUTRE LOUTRE\n");
    }
  else
    seq_printf(m, "Currently monitored PID:\n(Some of them might be dead already)\n");
  for (i = 0;i < ASE_CMD_MAX_PID; ++i)
    if (ase_cmd_pid_list[i] != 0)
    seq_printf(m, "%d\n", ase_cmd_pid_list[i]);
  return 0;
}

static int file_pid(struct seq_file *m, void *v)
{
  static struct task_struct	*task;
  int				n;

  n = 0;
  task = pid_task(ase_cmd_pid_struct[n], PIDTYPE_PID);
  if (task != NULL)
    {
      seq_printf(m, "Je suis une loutre ! %d",
		 cputime_to_usecs(task->utime + task->stime));
    }
 else
    {
      ase_cmd_pid_struct[n] = NULL;
      ase_cmd_pid_total = 0;
      ase_cmd_pid_list[n] = 0;
      printk(KERN_INFO "ASE_CMD: Task doesn't exist anymore");
      return -1;
    }
  return 0;
}

static int
ase_cmd_proc_open(struct inode *inode, struct file *file)
{
  printk(KERN_INFO "ASE_CMD: Open has been called");
  return single_open(file, ase_cmd_proc_show, NULL);
}

static ssize_t
ase_cmd_proc_write(struct file *filp, const char __user *buff,
		   size_t len, loff_t *data)
{
  long		res;
  struct pid	*tmp_pid;
  int		i;
  int		k;
  
  printk(KERN_INFO "ASE_CMD: Write has been called");
  if (len > (ASE_CMD_BUFFER_LEN - 1)) {
    printk(KERN_INFO "ASE_CMD: error, input too long");
    return -EINVAL;
  }
  else if (copy_from_user(ase_cmd_buffer, buff, len)) {
    printk(KERN_INFO "ASE_CMD: error while copying from userspace");
    return -2;
  }
  ase_cmd_buffer[len] = 0;

  kstrtol(ase_cmd_buffer, 0, &res);
  
  if (ase_cmd_pid_total >= ASE_CMD_MAX_PID)
    {
      printk(KERN_INFO "ASE_CMD: Max PID reached");
      return -3;
    }
  ++ase_cmd_pid_total;

  printk(KERN_INFO "ASE_CMD: Finding space in list");
  k = 0;
  // On va chercher une place dans la liste des pid et verifier que le pid est pas deja monitore
  for (i = 0;i < ASE_CMD_MAX_PID;++i)
    {
      if (ase_cmd_pid_struct[i] == NULL)
	k = i;
      if (ase_cmd_pid_list[i] == (int)res)
	{
	  printk(KERN_INFO "ASE_CMD: PID already monitored");
	  return -1;
	}
    }
  printk(KERN_INFO "ASE_CMD: getting pid struct");
    rcu_read_lock();
    tmp_pid = find_vpid((int)res);
    rcu_read_unlock();

    if (!tmp_pid)
      {
      printk(KERN_INFO "ASE_CMD: PID not found");
      return -1;
    }

  ase_cmd_pid_struct[k] = tmp_pid;
  ase_cmd_pid_list[k] = (int)res;
  printk(KERN_INFO "ASE_CMD: new pid registered");
  
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
  ase_proc_parent = proc_mkdir("ase", NULL);
    if (!ase_proc_parent)
    {
      printk(KERN_INFO "ASE_CMD: Error creating the directory /proc/ase");
      return -ENOMEM;
    }
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
