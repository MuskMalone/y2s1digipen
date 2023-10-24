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
    long int mtype;
    char mtext[MSG_LENGTH];
} MY_MSG;

int main(int argc, char** argv)
{
    int len, how;
    MY_MSG msg;

    if(argc !=3)
    {
	std::cerr << "Usage " << argv[0] << " queue_id msg_text" << std::endl;
	exit(1);
    }

    msg.mtype=0;
    strncpy(msg.mtext, argv[2], MSG_LENGTH);
    msg.mtext[1023]='\0';
    len=strlen(msg.mtext);
    std::cout << "Message sent is " << msg.mtext << std::endl;

    how = msgsnd(std::atoi(argv[1]), msg.mtext, len+1, IPC_NOWAIT);
    if(how==-1)
    {
	perror("Sending failed");
	exit(-2);
    }
		 
}


