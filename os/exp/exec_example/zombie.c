/* zombie.c
** -- creat a zombie process
** gcc zombie.c -o zombie
** run ./zombie & 
** use ps -elf to see the zombie status
** csd2180/2182 1/9/23
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
    if (fork() == 0) {
        /* Child */
        printf("Terminating Child, PID = %d, PPID = %d\n", getpid(), getppid());
        exit(0);
    } else {
        printf("Running Parent, PID = %d\n", getpid());
        sleep (1000);
    }

  return 0;
}
