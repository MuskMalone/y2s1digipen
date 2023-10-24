#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   // standard POSIX header file
#include <sys/wait.h> // POSIX header file for 'wait' function


void my_last_call()
{
    printf(" This should print last for a process.\n");
}

int main()
{
    int pid;
        
    pid=fork();
    if(pid==0)
    {
        int ppid = getppid();
        /* Informs the OS that my_last_call should be called before the child 
           process exit 
        */
        atexit(my_last_call);
        
        
        printf("Parent's PID is %d", ppid);
        printf(" Child process. ");
            
        /*
          Exiting the process by either
          calling 
              exit,
              _exit, 
              and return.
         */
        
        /*exit(100);*/
        _exit(10);
        /*return 100;*/
    }
    else
    {
        int status;
       
        printf("In parent process...\n");
        printf("ok.");
        wait(&status);
        
        printf("Parent process.\n");
    }
    
    return 0;
}
