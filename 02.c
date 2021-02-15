#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() 
{
    int p[2], ch[2], result;

    size_t size;
    char res[14];

    if (pipe(p) < 0) 
    {
        printf("Error open parent pipe\n");
        exit(-1);
    }

    if (pipe(ch) < 0)
    {
        printf("Error open child pipe\n");
        exit(-1);
    }

    result = fork();

    if (result < 0) 
    {
        printf("Can\'t fork\n");
        exit(-1);
    }

    else if (result > 0) 
    {
        close(p[0]);
        close(ch[1]);

        size = write(p[1], "Hello!", 14);

        if (size != 14) 
        {
            printf("Error write all string\n\r");
            exit(-1);
        }

        close(p[1]);

        size = read(ch[0], res, 14);

        if (size != 14)
        {
            printf("Error read from child\n\r");
            exit(-1);
        }

        close(ch[0]);

        printf("Parent reads: %s\n\r", res);
    }

    else
    {
       
        close(p[1]);
        close(ch[0]);

        size = read(p[0], res, 14);

        if (size < 0)
        {
            printf("Error get from parent\n\r");
            exit(-1);
        }

        printf("Child reads: %s\n\r", res);

        size = write(ch[1], res, 14);

        close(ch[1]);
        close(p[0]);
    }

    return 0;
}
