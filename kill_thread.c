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
asmlinkage long sys_killing(int tid[],int n)
{
	int m,i,rtn;
	for(i=0;tid[i]!=0;i++);
	i--;
	for( m=0;m<n&&i!=0;m++,i--)
	{
		sys_kill(tid[i],SIGTERM);
		tid[i]=0;
	}
	return rtn;
}
MODULE_LICENSE("GPL");

