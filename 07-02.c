#include <stdio.h>
#include <errno.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include "stdlib.h"

int main()
{
    int shmid;
    key_t key;
    char path[] = "07-02.c";

    if ((key = ftok(path,0)) < 0) {

        printf("Unable generate key\n");
        exit(-1);
    }

    int size = 0;

    if ((shmid = shmget(key, sizeof(int) + size * sizeof(char), 0666|IPC_CREAT)) < 0) {
        
        printf("Unable create shared memory\n");
        exit(-1);
    }

    int *size_ptr;

    if ((size_ptr = (int *)shmat(shmid, NULL, 0)) == (int *)(-1)) {
        
        printf("Unable attach shared memory\n");
        exit(-1);
    }

    size = *size_ptr;
    char *text;
    text = (char*)(size_ptr + 1);
    
    for (int i = 0; i < size; i++){
       
        putchar(text[i]);
    }
        
    if (shmdt(size_ptr) < 0) {
        
        printf("Unable detach shared memory\n");
        exit(-1);
    }
    
    if (shmctl(shmid, IPC_RMID, NULL) < 0) {
        
        printf("Unable delete shared memory\n");
        exit(-1);
    }
    return 0;
}
