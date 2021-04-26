#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255
int main(void)
{
  int msqid; 

  char pathname[]="11-1a1.c"; 

  key_t  key;     
  int i,len;      

  struct mymsgbuf 
  {
    long mtype;

    struct 
    {
      int fMessage;
      short sMessage;
    } mes;
  } mybuf;

  if ((key = ftok(pathname,0)) < 0) {
    
      printf("Can\'t generate key\n");
    exit(-1);
  }
 

  if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
    printf("Can\'t get msqid\n");
    exit(-1);
  }

  printf("Program 1 started and send 5 messages to 2 program.\n");
  for (i = 1; i <= 5; i++) {
   
    mybuf.mtype = 1;
    mybuf.mes.fMessage = 2021;
    mybuf.mes.sMessage = 1337;
    len = sizeof(mybuf.mes);
  
    if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) {
      printf("Can\'t send message to queue\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
      exit(-1);
    }
  }

  printf("Program sent messages.\n");


  for (size_t i = 1; i <= 6; i++)
  {
    len = sizeof(mybuf.mes);
    if (msgrcv(msqid, (struct msgbuf *) &mybuf, len, 2, 0) < 0) {
      printf("Can\'t receive message from queue\n");
      exit(-1);
    }
    printf("Program recieved message: message type = %ld, fMessage = %i, sMessage = %i\n", mybuf.mtype, mybuf.mes.fMessage, mybuf.mes.sMessage);
  }

  msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);

  return 0;
}

