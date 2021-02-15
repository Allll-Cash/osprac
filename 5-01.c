#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    int f;
    size_t size;

    const int bytes = 14;
    const char* file = "myfile";

    (void) umask(0);

    if ((f = open(file, O_RDONLY)) < 0)
    {
        printf("Error open file\n");
        exit(-1);
    }
    char d[bytes];

    size = read(f, d, bytes);

    if (size != bytes)
    {
        printf("Error read\n");
        exit(-1);
    }

    printf("%s\n\r", d);

    if (close(f) < 0)
    {
        printf("Error\n");
    }

    return 0;
}
