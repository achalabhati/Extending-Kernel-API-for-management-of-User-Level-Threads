#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/unistd.h>
#include <linux/syscalls.h>
#include <linux/unistd.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <asm/uaccess.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/proc_fs.h>
#include <linux/pid.h>
#include <linux/pid_namespace.h>


asmlinkage long sys_example(int sch,int no,void (*fn)(void),char ***child_stack,int flags,int shmid,int tid[])
{
	struct sched_param p;
	struct sched_param *a =&p;
	long rtn=0,i=0,count;
	struct pid *pst;
	struct task_struct *tsk;

	for(count=0;tid[count]!=0;count++);

	
	for(i=0;i<no;i++)
	{
		rtn = sys_clone(flags,*(child_stack+i)+4096-1,NULL,NULL,fn);
		tid[count]=rtn;
		count++;
		printk("Thread %d:%d\n",i,rtn);
		pst= find_get_pid(rtn);
		tsk=pid_task(pst,PIDTYPE_PID);
		a-> sched_priority=1;

		printk("Thread after .. %d:%ld\n",i,tsk->pid);
		//sched_setparam(rtn,a);
		sched_setscheduler(tsk,sch,a);
		sys_shmat(shmid,NULL,0);
	}
	return rtn;
}
MODULE_LICENSE("GPL");

