#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int main()
{
pid_t pid, ppid;

pid = getpid();
fork();

if (getppid() == -1)
{
    perror("fork");
     exit(1);
} else if (pid == 0)
{
     printf("PID ребенка  %d\n", (int)getpid());
} else 
{
     printf("PID родителя  %d\n",(int)getpid());
}
return 0;
}
