#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int main()
{
pid_t pid, ppid;


(void)fork();

pid = fork();

if (pid == -1) {
    perror("fork");
     exit(1);
} else if (pid == 0) {
     printf("PID ребенка  %d\n", getpid());
} else {
     printf("PID родителя  %d\n", getpid());
}
return 0;
}
