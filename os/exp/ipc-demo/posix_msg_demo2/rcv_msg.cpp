#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>

#define MSG_LENGTH 1024

typedef struct MSG_FORMAT
{
    int mtype;
    char mtext[MSG_LENGTH];
} MY_MSG;

int main(int argc, char** argv)
{
    int len, how;
    MY_MSG msg;

    if(argc !=2)
    {
	     std::cerr << "Usage " << argv[0] << " queue_id" << std::endl;
	     exit(1);
    }

    len = msgrcv( std::atoi(argv[1]), msg.mtext, MSG_LENGTH, 0, 0);
    
    if(len==-1)
    {
		perror("Receiving failed");
		exit(-2);
    }
    else
		std::cout << "Receieved message is " << msg.mtext << std::endl; 
}


