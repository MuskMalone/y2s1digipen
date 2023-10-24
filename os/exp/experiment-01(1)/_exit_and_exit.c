#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   // standard POSIX header file
#include <sys/wait.h> // POSIX header file for 'wait' function


int main()
{
    int pid;
        
    pid=fork();
    if(pid==0)
    {        
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
    }
    else
    {
        int status;
       
        printf("In parent process...\n");
        wait(&status);
        
        printf("Done.\n");
    }
    
    return 0;
}
