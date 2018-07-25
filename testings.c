#include<linux/unistd.h>
#include<sys/types.h>
#include<sys/syscall.h>
#include<sys/sem.h>
#include<sched.h>
#include<linux/sched.h>
#include<sys/shm.h>
#include<sys/stat.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#define __NR_mycall 325
#define _SCHED_H 1
#define __USE_GNU 1
#define STACK_SIZE 4096
#define SHM_SIZE 1024



int shmid;
int sem_id,arr[100];
void sem_lock(int sem_set_id)
{
    /* structure for semaphore operations.   */
    struct sembuf sem_op;

    /* wait on the semaphore, unless it's value is non-negative. */
    sem_op.sem_num = 0;
    sem_op.sem_op = -1;
    sem_op.sem_flg = 0;
    semop(sem_set_id, &sem_op, 1);
}

void sem_unlock(int sem_set_id)
{
    /* structure for semaphore operations.   */
    struct sembuf sem_op;

    /* signal the semaphore - increase its value by one. */
    sem_op.sem_num = 0;
    sem_op.sem_op = 1;   /* <-- Comment 3 */
    sem_op.sem_flg = 0;
    semop(sem_set_id, &sem_op, 1);
}

int func1(void *arg) 
{
	void *one=shmat(shmid,NULL,0);
	char *s,*one1=(char *)one;
    	printf("Inside function A\nThread id:%ld\n",syscall(SYS_gettid));
	sem_lock(sem_id);
	strncpy(one1,"achala",3);
	sem_unlock(sem_id);
  	printf(".A\n");
	printf(".A\n");
 	printf(".A\n");
    	printf("Terminating func A\n");
	sleep(10);
    	_exit(1);
}

int func2(void *arg) 
{
	void *two=shmat(shmid,NULL,0);	
	char *s,*two2=(char *)two;
    	printf("Inside function B\nThread id: %ld\n",syscall(SYS_gettid));
	printf(".B\n");
 	printf(".B\n");
	printf(".B\n");
    	sem_lock(sem_id);
	for(s=two2;*s!=NULL;s++)
	printf("%c",*s);
	printf("\n");
	sem_unlock(sem_id);
	printf("Terminating func B\n");
	sleep(10);
  	_exit(1);
}


void create(int number)
{
	int sch=1,sem_id,rc,i;
	union semun{
		int val;
		struct semid_ds *buf;
		ushort *array;
		} sem_val;
	char ***child_stack,**p;
	child_stack=(char ***) malloc(number);
	
	for(i=0;i<number;i++)
	{
		child_stack[i] = (char **)malloc(STACK_SIZE);
		p=(child_stack[i]+4096-1);
		if(i%2==0)
		*p=(char *)&func1;
		else
		*p=(char *)&func2;		
	}
	sem_id=semget(250,1,IPC_CREAT|0600);
	sem_val.val=1;
	printf("This process pid: %u\n", getpid());
	rc=semctl(sem_id,0,SETVAL,sem_val);	
	shmid=shmget(IPC_PRIVATE,SHM_SIZE,IPC_CREAT|IPC_EXCL|S_IRUSR|S_IWUSR);
	syscall(325,sch,number,&func1,child_stack,CLONE_SIGHAND|CLONE_FS|CLONE_VM|CLONE_FILES|CLONE_THREAD,shmid,arr);
	
}

void kill_thread(int number)
{	
	syscall(326,arr,number);	
}


int main()
{
	int ret,number=5;
	printf("This process pid: %u\n", getpid());
	printf("This process policy: RR\n");
	
	create(2);
	kill_thread(3);	
	sleep(10);
}

