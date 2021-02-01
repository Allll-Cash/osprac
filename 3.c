#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    printf("Pid = %d", (int)getpid());
    printf("Ppid = %d", (int) getppid());

    return 0;
}
