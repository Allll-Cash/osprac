#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

    while (1) {
        if (msgrcv(msqid, (struct Client *) &client, sizeof(client.message), 1, 0) < 0) {
            printf("Can\'t receive message from queue\n");
            exit(-1);
        }

        printf("Client sent message: %lf, pid = %d\n", client.message.num, client.message.pid);

        server.message.num = client.message.num * client.message.num;
        server.mtype = client.message.pid;

        if (msgsnd(msqid, &server, sizeof(server.message), 0) < 0) {
            printf("Can\'t send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            exit(-1);
        }

        printf("Sent message to client: %lf.\n", server.message.num);
    }
    return 0;
}
