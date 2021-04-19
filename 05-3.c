#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


 /*Начальное значение семафора = 1
 После записи сообщения в цикле в родительском процессе счетчик уменьшаетс вызывая операцию D,
 этим он блокирует семафор и дает дочернему процессу читать сообщение.
 После этого и записи сообщения счетчик увеличивается вызывая операцию A, этим он снимает блокировку с семафора,
 позволяя родительскому процессу читать сообщение.*/
 
int A(int sem, struct sembuf* buf)
{
    buf->sem_op = 1;
    buf->sem_flg = 0;
    buf->sem_num = 0;

    return semop(sem, buf, 1);
}

int D(int sem, struct sembuf *buf) 
{
    buf->sem_op = -1;
    buf->sem_flg = 0;
    buf->sem_num = 0;

    return semop(sem, buf, 1);
}

int main() 
{
    int N, i;

    printf("Enter count of iterations (>2):\n> ");
    scanf("%d", &N);

    if (N < 2) 
    {
        printf("incorrect! N = 2\n");
        N = 2;
    }

    char pathname[] = "task03.c";

    key_t key;

    int semid;
    struct sembuf mybuf;

    int fd[2], result;

    int size;
    char resstring[16];

    if (pipe(fd) < 0)
    {
        printf("Can\'t open pipe\n");
        exit(-1);
    }

    if ((key = ftok(pathname, 0)) < 0) 
    {
        printf("Can\'t generate key\n");
        exit(-1);
    }

    if ((semid = semget(key, 1, 0666)) < 0)
    {
        if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0)
        {
            printf("Can\'t create semaphore set\n");
            exit(-1);
        }

        printf("Success\n");
    }

    result = fork();

    for (i = 0; i < N; ++i)
    {
        if (result < 0) 
        {
            printf("Can\'t fork child[%d]\n", i);
            exit(-1);
        } 
        else if (result > 0) 
        {
            printf("parent[%d]: start\n", i);

            size = write(fd[1], "Hello, world![p]", 16);

            if (size != 16) 
            {
                printf("parent[%d]: can\'t write all string to pipe\n", i);
                exit(-1);
            }
            printf("parent[%d]: wrote message to child\n", i);

            D(semid, &mybuf);

            size = read(fd[0], resstring, 16);

            if (size < 0)
            {
                printf("parent[%d]: can\'t read string from pipe\n", i);
                exit(-1);
            }

            printf("parent[%d]: read message from child\n> %s\n", i, resstring);

            printf("parent[%d]: end\n", i);
        } 
        else
        {

            printf("child[%d]: start\n", i);

            size = read(fd[0], resstring, 16);

            if (size < 0) 
            {
                printf("child[%d]: can\'t read string from pipe\n", i);
                exit(-1);
            }

            printf("child[%d]: read message from parent\n> %s\n", i, resstring);

            size = write(fd[1], "Hello, world![с]", 16);

            if (size != 16) 
            {
                printf("child[%d]: can\'t write all string to pipe\n", i);
                exit(-1);
            }
            printf("child[%d]: wrote message to parent\n", i);

            A(semid, &mybuf);

            printf("child[%d]: end\n", i);
        }
    }
    close(fd[0]);
    close(fd[1]);

    return 0;
}
