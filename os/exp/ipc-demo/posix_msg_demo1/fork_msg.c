/* msg.c
** -- POSIX message queues
** csd2180/csd2182 9/23
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/msg.h>


typedef struct {
  long int type;//8 bytes in x64
  char buffer[1024];
} msg_struct;

#define MSG_STRUCT 1


int main(void) {
  int pid, queue_id;
  msg_struct msg;

  pid = fork();
  if (pid == 0) {
    queue_id = msgget((key_t)100, 0666 | IPC_CREAT);
    msg.type = MSG_STRUCT;
    strcpy(msg.buffer,"Was it a rat I saw?");
    msgsnd(queue_id, &msg, 1024, 0);
    strcpy(msg.buffer, "No lemons, no melon.");
    msgsnd(queue_id, &msg, 1024, 0);    
	fprintf(stdout,"child process %d finish sending message %d\n", getpid(), queue_id);	
    exit(0); }
  else {
    wait(NULL);
    queue_id = msgget((key_t)100, 0666 | IPC_CREAT);
	/*If the IPC_NOWAIT flag is set in the msgflg parameter, 
	 * msgrcv() returns immediately with a return value of -1 
	 * and errno set to ENOMSG.
	 */

	/*If the IPC_NOWAIT flag is not set in the msgflg parameter, 
	 *the calling thread suspends processing until one of the following occurs:

	 *A message of the desired type is sent to the message queue.

	 *The message queue identifier msqid is removed from the system. 
	 * When this occurs, the msgrcv() function returns with a return value 
	 * of -1 and errno set to EIDRM.

	 *A signal is delivered to the calling thread. When this occurs, 
	 * the msgrcv() function returns with a return value of -1 
	 * and errno set to EINTR.	
	 */	
	fprintf(stdout,"parenet process %d is waiting for message %d\n", getpid(), queue_id); 
    while (msgrcv(queue_id, &msg, 1024, MSG_STRUCT, IPC_NOWAIT) != -1) {
      if (msg.type == MSG_STRUCT)
        fprintf(stdout,"message: %s\n",msg.buffer);
      else
        fprintf(stdout,"unknown message\n");
    }
  }

  msgctl(queue_id,IPC_RMID,NULL);
  return 0;
}
