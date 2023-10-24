#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>

int main(int argc, char** argv)
{
    int qid;

    if( argc !=2 ) 
    {
	std::cerr << "Usage: " << argv[0] << " queue_key " << std::endl;
	exit(-1);
    }
    
    if((qid = msgget(atoi(argv[1]), IPC_CREAT | 0600)) == -1) 
    {
	perror("cannot create message queue");
	exit(2);
    }
    //std::cout << "Message queue id is " << qid <<  std::endl;
    std::cout << qid ;

}
