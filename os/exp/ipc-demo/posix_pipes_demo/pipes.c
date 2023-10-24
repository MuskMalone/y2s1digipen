/* pipes.c
** -- using POSIX pipes
** csd2180/2182 9/23
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
/*
       #include <unistd.h>

       int pipe(int pipefd[2]);

       #define _GNU_SOURCE             /// See feature_test_macros(7) 
       #include <fcntl.h>              /// Definition of O_* constants 
       #include <unistd.h>

       int pipe2(int pipefd[2], int flags);
       pipe() creates a pipe, a unidirectional data channel that can be
       used for interprocess communication.  The array pipefd is used to
       return two file descriptors referring to the ends of the pipe.
       pipefd[0] refers to the read end of the pipe.  pipefd[1] refers
       to the write end of the pipe.  Data written to the write end of
       the pipe is buffered by the kernel until it is read from the read
       end of the pipe.
	   
	   
       The dup() system call allocates a new file descriptor that refers
       to the same open file description as the descriptor oldfd.  (For
       an explanation of open file descriptions, see open(2).)  The new
       file descriptor number is guaranteed to be the lowest-numbered
       file descriptor that was unused in the calling process.	   
*/	   
int main(void) 
{

	int pid;
	char buffer[1024];
	int descriptor[2];
	/* descriptor[0] - for reading
	** descriptor[1] - for writing
	*/
	
	// pipe() is a system call returns 2 file descriptors
	// 
	pipe(descriptor);
	pid = fork();
	
	if (pid == 0) {
		fprintf(stdout,"Child getting Input: ");
		fgets(buffer,sizeof(buffer),stdin);
		//system call that writes to a file 
		
		close(1);
		dup(descriptor[1]);
		fprintf(stderr, "Child received \"%s\"\n", buffer);
		printf("%s\n", buffer);
		//write(1,buffer,strlen(buffer)+1);
		//write(descriptor[1],buffer,strlen(buffer)+1);
		fflush(stdout);
		exit(0); 
	}
	else {
		int bytes_read;
		while(1)
		{
			bytes_read = read(descriptor[0],buffer,sizeof(buffer));
			buffer[bytes_read]=0;

			if(bytes_read>0)
			{     
				fprintf(stdout,"Parent reading message:\"%s\", %i\n", buffer, bytes_read);
				buffer[0]=0;
				break;
			}
		}
	}
	
	close(descriptor[0]);
	close(descriptor[1]);
	return 0;
}
