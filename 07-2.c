#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int b = 0;

void *mythread(void *dummy)
{
    pthread_t mythid;
    mythid = pthread_self();
    b = b + 1;
    printf("Thread %d, res = %d\n", mythid, b);
    return NULL;
}

int main()
{
    pthread_t thid, thid2, mythid;
    int res;

    res = pthread_create( &thid, (pthread_attr_t *)NULL, mythread, NULL);
    if (res != 0) {
        printf ("Error on thread create, return value = %d\n", res);
        exit(-1);
    }
    printf("2nd thread created, thid = %d\n", thid);

    res = pthread_create( &thid2, (pthread_attr_t *)NULL, mythread, NULL);
    if (res != 0) {
        printf ("Error on thread create, return value = %d\n", res);
        exit(-1);
    }
    printf("3rd thread created, thid = %d\n", thid2);

    mythid = pthread_self();
    b = b + 1;
    printf("Thread %d, Calculation result = %d\n", mythid, b);

    pthread_join(thid, (void **)NULL);
    pthread_join(thid2, (void **)NULL);
    return 0;
}
