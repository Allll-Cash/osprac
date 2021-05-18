#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

struct Client {
    long mtype;
    struct {
        pid_t pid;
        double num;
    } message;
} client;

struct Server {
    long mtype;
    struct {
        double num;
    } message;
} server;


int main(void) {
    int msqid;
    char pathname[] = "11-3b.c";
    key_t key;

    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    }

    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Can\'t get msqid\n");
        exit(-1);
    }


    srand(time(NULL));

    client.message.pid = getpid();
    client.message.num = ((double) rand()) / RAND_MAX * 1000;
    client.mtype = 1;

    if (msgsnd(msqid, &client, sizeof(client.message), 0) < 0) {
        printf("Can\'t send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        exit(-1);
    }

    printf("Send message; type = %ld, message = '%lf'\n", client.mtype, client.message.num);

    printf("Wait\n");

    if (msgrcv(msqid, &server, sizeof(server.message), getpid(), 0) < 0) {
        printf("Can\'t receive message from queue.\n");
        exit(-1);
    }

    printf("Message from the server: %lf\n", server.message.num);

    return 0;
}
