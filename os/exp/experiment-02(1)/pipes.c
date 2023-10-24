#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char** argv) 
{

	int pid;	
	int descriptor[2];
	char buffer[1024];
		
	pipe(descriptor);
	
	/*
		descriptor[0] - reading end
		descriptor[1] - writing end
	*/
	
	
	pid = fork();
	
	if (pid == 0) 
	{	
		fgets(buffer, 1024, stdin); 
		
		while(1)
		{
			write(descriptor[1],buffer,strlen(buffer));	
			fprintf(stdout,"Child writing message:\"%s\"\n", buffer);	
			sleep(1);
		}
		exit(0);
	}
	else 
	{			
		
		int bytes_read;
	
		while(1)
		{			
			bytes_read=read(descriptor[0],buffer, 10);
			buffer[bytes_read]='\0';
			if(bytes_read>0)
			{     
				fprintf(stdout,"Parent reading message:\"%s\"\n", buffer);			
			}
			sleep(1);
		}
	}
	
	close(descriptor[0]);
	close(descriptor[1]);
	return 0;
}
