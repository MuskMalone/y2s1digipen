#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   // standard POSIX header file
#include <sys/wait.h> // POSIX header file for 'wait' function


void my_last_call()
{
    printf(" This should be printed last for a process.\n");
}

int main()
{
    
    /* 
       Informs the C library that my_last_call 
       should be called before my process 
       finally exit.
    */
    atexit(my_last_call);
    
           
    /*
      Exiting the process by either
      calling exit, _exit, 
      or return.
    */
    
    exit(100);
    /*_exit(100);*/
    /*return 100;*/
}
