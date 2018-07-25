# Extending-Kernel-API-for-management-of-User-Level-Threads
Extending the kernel API for creation and correct management of user-level threads. Using this API, a user level program must be able to create, terminate one or more threads. Furthermore a user level program will be able to specify different scheduling policies for a group of threads. The API will also export basic primitives for safe inter-thread communication.

# Approach
For thread creation we have created a system call create, in which we are passing no of threads to create. In order to create each thread we use sys_clone function in our system call. Also we have passed the user level functions we created, to the system call, which will be executed by new created threads.
Then we have done thread scheduling using sched_set_scheduler, then we have passed scheduling policies and priority. We have executed threads on different scheduling policies and different priorities.
After that in order to do communication between threads we have use shared memory. We created shared memory using shmget() that is shared between the created threads. Different thread can read and write in this shared memory. So as all threads can try to read or write at the same time, for this purpose we need some synchonization method to provide safe communication between the threads. In order provide synchronization we have used semaphores, that will provide locks on conflicting operation.
Now at the end we make a system call to terminate a no of threads, this system call has parameters i.e. no of threads to terminate and a thread array that will keep track which threads are alive. Now in order to kill individual thread we use sys_kill method of the kernel library.
