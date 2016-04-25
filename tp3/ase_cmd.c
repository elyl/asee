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

struct ase_cmd_pid
{
  struct pid		*pid_s;
  int			pid_n;
  struct proc_dir_entry	*pid_proc;
};

#define ASE_CMD_BUFFER_LEN 10
static int		buffer_mutex;
static char		ase_cmd_buffer[ASE_CMD_BUFFER_LEN];

#define ASE_CMD_MAX_PID 10
static struct ase_cmd_pid	ase_cmd_list[ASE_CMD_MAX_PID];

static int		ase_cmd_pid_total = 0;

static int			pid_mutex = 0;
static struct ase_cmd_pid	*pid_entry_current;

static struct proc_dir_entry	*ase_proc_parent;

static void
clean_buffer(void)
{
  int	i;

  i = 0;
  while (ase_cmd_buffer[i])
    {
      if ((ase_cmd_buffer[i] < '0' || ase_cmd_buffer[i] > '9') && ase_cmd_buffer[i] != ' ')
	ase_cmd_buffer[i] = '\0';
      ++i;
    }
}

static void
buffer_lock(void)
{
  while (buffer_mutex == 1)
    ;
  buffer_mutex = 1;
}

static void
buffer_unlock(void)
{
  buffer_mutex = 0;
}

static void
pid_lock(void)
{
  printk(KERN_INFO "ASE_CMD: Waiting for unlocking");
  while (pid_mutex == 1)
    ;
  printk(KERN_INFO "ASE_CMD: Lock has beek taken");
  pid_mutex = 1;
  return;
}

static void
pid_unlock(void)
{
  pid_mutex = 0;
  return;
}

static int
ase_cmd_proc_show(struct seq_file *m, void *v)
{
  int	i;

  if (ase_cmd_pid_total == 0)
    {
      seq_printf(m, "You are not monitoring any PID right now\n");
    }
  else
    seq_printf(m, "Currently monitored PID:\n(Some of them might be dead already)\n");
  for (i = 0;i < ASE_CMD_MAX_PID; ++i)
    if (ase_cmd_list[i].pid_s != NULL)
    seq_printf(m, "%d\n", ase_cmd_list[i].pid_n);
  return 0;
}

static int
ase_cmd_proc_pid_show(struct seq_file *m, void *v)
{
  static struct task_struct	*task;
  struct ase_cmd_pid		*s;

  s = pid_entry_current;
  pid_unlock();
  task = pid_task(s->pid_s, PIDTYPE_PID);
  if (task != NULL)
    {
      seq_printf(m, "PID %d, execution time:\n user: %d ms\n system : %d ms\n",
		 s->pid_n,
		 cputime_to_usecs(task->utime),
		 cputime_to_usecs(task->stime));
    }
  else
    {
      s->pid_s = NULL;
      s->pid_n = 0;
      proc_remove(s->pid_proc);
      s->pid_proc = NULL;
      printk(KERN_INFO "ASE_CMD: Task doesn't exist anymore");
      return -1;
    }
  return 0;
}

static int
ase_cmd_proc_pid_open(struct inode *inode, struct file *file)
{
  printk(KERN_INFO "ASE_CMD: Open pid has been called");
  pid_lock();
  pid_entry_current = (struct ase_cmd_pid*)PDE_DATA(inode);
  return single_open(file, ase_cmd_proc_pid_show, file);
}

static int
ase_cmd_proc_open(struct inode *inode, struct file *file)
{
  printk(KERN_INFO "ASE_CMD: Open has been called");
  return single_open(file, ase_cmd_proc_show, NULL);
}

static int
ase_cmd_add_pid(long res)
{
  struct pid	*tmp_pid;
  int		i;
  int		k;
  
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
      if (ase_cmd_list[i].pid_s == NULL)
	k = i;
      if (ase_cmd_list[i].pid_n == (int)res)
	{
	  --ase_cmd_pid_total;
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

  ase_cmd_list[k].pid_s = tmp_pid;
  ase_cmd_list[k].pid_n = (int)res;
  printk(KERN_INFO "ASE_CMD: new pid registered");
  return k;
}

static const struct file_operations ase_cmd_proc_pid_fops = {
  .owner      = THIS_MODULE,
  .open       = ase_cmd_proc_pid_open,
  .read       = seq_read,
  //  .write      = seq_write,
  .llseek     = seq_lseek,
  .release    = single_release,
};
static int
ase_cmd_new_file(int n)
{
  struct proc_dir_entry	*entry;
  
  entry = proc_create_data(ase_cmd_buffer, 0666, ase_proc_parent, &ase_cmd_proc_pid_fops, &ase_cmd_list[n]);
  if (!entry)
    {
      printk(KERN_INFO "ASE_CMD: Error creating pid file");
      return -1;
    }
  ase_cmd_list[n].pid_proc = entry;
  printk(KERN_INFO "ASE_CMD: Created new PID file");
  return 0;
}

static ssize_t
ase_cmd_proc_write(struct file *filp, const char __user *buff,
		   size_t len, loff_t *data)
{
  long		res;
  int		r;
  
  printk(KERN_INFO "ASE_CMD: Write has been called");
  if (len > (ASE_CMD_BUFFER_LEN - 1)) {
    printk(KERN_INFO "ASE_CMD: error, input too long");
    return -EINVAL;
  }
  buffer_lock();
  if (copy_from_user(ase_cmd_buffer, buff, len)) {
    printk(KERN_INFO "ASE_CMD: error while copying from userspace");
    return -2;
  }
  ase_cmd_buffer[len] = '\0';
  clean_buffer();

  kstrtol(ase_cmd_buffer, 0, &res);

  if ((r = ase_cmd_add_pid(res)) < 0)
    return r;
  if ((r = ase_cmd_new_file(r)) < 0)
    return r;

  buffer_unlock();
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

// Ajouter clean des fichiers a la sortie, necessaire?
static void __exit
ase_cmd_proc_exit(void)
{
  int	i;
  
  remove_proc_entry("ase_cmd", NULL);

  for (i = 0; i < ASE_CMD_MAX_PID; ++i)
    {
      if (ase_cmd_list[i].pid_proc != NULL)
	proc_remove(ase_cmd_list[i].pid_proc);
    }
  remove_proc_entry("ase", NULL);
}

module_init(ase_cmd_proc_init);
module_exit(ase_cmd_proc_exit);

MODULE_LICENSE("GPL");
