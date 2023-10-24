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


int main(void) {
    int pid;
    int shmid;
    char *buffer;
    key_t mem_key = 100;
    
    sleep(3);
    shmid = shmget(mem_key,1024,0666|IPC_CREAT);
    buffer = (char*)shmat(shmid,NULL,0);
    fprintf(stdout,"message: %s\n",buffer);
    shmdt(buffer);
    shmctl(shmid,IPC_RMID,0);
    return 0;
}
