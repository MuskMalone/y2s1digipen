#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   // standard POSIX header file
#include <sys/wait.h> // POSIX header file for 'wait' function

int main()
{
    int pid;
    
    pid	= getpid(); /* Parent's PID */	
    printf("Original Process's  PID = %d\n", pid);
    
    pid=fork();
    if(pid==0)
    {
        int ppid = getppid();
        printf("Parent's PID is %d\n", ppid);
        printf("Child process.\n");
    }
    else
    {
        printf("Parent process.\n");
    }
    
    return 0;
}
