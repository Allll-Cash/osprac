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
  int len, maxlen;

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

  printf("Program 2 started.\n");

  for (size_t i = 1; i <= 5; i++)
  {
    len = sizeof(mybuf.mes);
    if (msgrcv(msqid, (struct msgbuf *) &mybuf, len, 1, 0) < 0) {
      printf("Can\'t receive message from queue\n");
      exit(-1);
    }
    printf("Program recieved message: message type = %ld, fMessage = %i, sMessage = %i\n", mybuf.mtype, mybuf.mes.fMessage, mybuf.mes.sMessage);
  }

  printf("Program send 6 messages to 1 program.\n");
  for (int i = 1; i <= 6; i++) {
   
    mybuf.mtype = 2;
    mybuf.mes.fMessage = 1991;
    mybuf.mes.sMessage = 137;
    len = sizeof(mybuf.mes);
    //
   
    if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) {
      printf("Can\'t send message to queue\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
      exit(-1);
    }
  }

  printf("Program sent messages.\n");
  
  return 0;
}

