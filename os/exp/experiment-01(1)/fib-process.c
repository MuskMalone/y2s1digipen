#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   // standard POSIX header file
#include <sys/wait.h> // POSIX header file for 'wait' function

int main(int argc, char **argv)
{
    int num;
    int is_child = 0;
    int status, res1, res2;

    if(argc != 2)
    {
	printf("%s <number>", argv[0]);
	return 1;
    }

    num = atoi(argv[1]);

    if(num < 0)
    {
	printf("%s <number>", argv[0]);
	printf("number must be positive!");
	return 1;
    }

    while(num > 1)
    {	
	if(fork()==0)
	{
	    is_child=1;
	    num--;
	    continue;
	}
	else 
	{
	    if(fork()==0)
	    {
		is_child=1;
		num-=2;
		continue;
	    }	    
	}
	break;
    }
    if(num > 1)
    {
	wait(&status);
	res1 = WEXITSTATUS(status);
	wait(&status);
	res2 = WEXITSTATUS(status);
	if(is_child)
	    return res1+res2;
	else
	    printf("Result is %d\n", res1+res2);
    } 
    else
    {
	if(is_child)
	    return num;
	else
	    printf("Result is %d\n", num);
    }
}
