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
        //Child Process Code
        printf("Child Process.");
        int ppid = fork();
        if (ppid == 0){
            exit(123);
        }else{
            int status;
            wait(&status);
            printf("Child hahahaProcess. %d\n", status);

            exit(WEXITSTATUS(status));
        }
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
        
        if(WIFEXITED(status))
            printf("status = %d\n", WEXITSTATUS(status));
        
        printf("Parent Process. %d\n", status);
    }
    
    return 0;
}
