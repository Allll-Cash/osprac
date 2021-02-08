#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
 
int main(void) 
{
    switch(fork())
  {
        case -1:
            perror("fork");
            return 1;
        case 0:
            if ( execl("/usr/bin/man", "man", "execl", NULL) == -1 )
           {
                perror("execl");
                return 1;
            }
            break;
        default:
            wait(NULL);
            printf("End program!");
            break;
    }
    
    return 0;
}
