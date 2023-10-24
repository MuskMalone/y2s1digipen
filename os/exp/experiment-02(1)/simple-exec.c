#include "unistd.h"
#include <stdio.h>
int main()
{
	/* 
		Calling the ls utility program:
		Comment out the others and keep only one 
		un-commented for the experiments 
	*/
	execl("/bin/ls", "ls", "-l", NULL);
	//execl("ls", "ls", "-l", NULL);
	//execlp("ls", "ls", "-l", NULL);
	//execlp("/bin/ls", "ls", "-l",NULL);
	
	printf("If execution reaches here, exec has failed.\n");
}