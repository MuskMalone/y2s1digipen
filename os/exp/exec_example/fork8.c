/* fork8.c
** -- creat a child process running for ever while
**    parent goes off.
** csd2180/2182 1/9/23
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
int main(void) 
{
    if (fork() == 0) {
        /* Child */
        printf("Running Child, PID = %d, PPID = %d\n",
               getpid(), getppid());
        while (1)
            ; /* Infinite loop */
    } else {
        printf("Terminating Parent, PID = %d\n",
               getpid());
        exit(0);
    }
	return 0;
}