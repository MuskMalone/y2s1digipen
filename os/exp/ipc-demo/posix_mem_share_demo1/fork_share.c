/* share.c
** -- shared memory using POSIX
** csd2180/2182 9/23
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>

/*Linux : use 'ipcs -m' to show*/

int main(void) {
    int pid;
    int shmid;
    char *buffer;
    key_t mem_key = 100;
    
    pid = fork();
    if (pid == 0)
    {
		/*shmget
		-- shmget(key_t, size, permissions_flag|flags);
		
		-- Creates a chunk of memory for sharing IF
			-- the memory associated with the key doesn't exist yet.
		-- Every shared memory is associated with 
			-- ID and key.
			-- ID only known to OS.
			-- key is known to all who share the same memory.
		-- shmget returns the ID.
		*/

		shmid = shmget(mem_key, 1024, 0666|IPC_CREAT);
		/*
		  shmat
		  -- Attach Shared Memory to process.
		  -- returns a pointer (void *)
			  -- second parameter you provide an address you like to see returned.
		  -- if you are not particular about the address, NULL is sufficient. 
		*/
		//shmdt 
		
		buffer = (char*)shmat(shmid,NULL,0);
		strcpy(buffer,"step haha on no pets");
		/* 
		   -- shmdt (detached the memory chunk from memory) 
		 */
		shmdt(buffer);
		exit(0); 
    }
    else {
		wait(NULL);
		shmid = shmget(mem_key, 1024, 0666|IPC_CREAT);
		buffer = (char*)shmat(shmid,NULL,0);
		fprintf(stdout,"message: %s\n",buffer);
		shmdt(buffer);
    }
    /*
      shared memory control 
      -- where you can issue multiple sorts of commands.
      -- removing the shared memory from the system.
      -- Shared memory continues to exist outside the process.
     */
    shmctl(shmid, IPC_RMID, 0);
	/*remove the above line and use 'ipcs -m' to view share memory*/
	/*use 'ipcs -m -i shmid' to view share memory info with id being shmid*/
	/*use 'ipcrm shm shmid' to remove share memory with id being shmid*/
    return 0;
}
