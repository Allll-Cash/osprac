#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[], char **envp)
{
      for (int i = 0; i < argc; i++) 
       {
        printf("Argument [%d]: %s\n", i, argv[i]);
       }
    while (*envp)
    {
        printf("Environment: %s\n", *envp++);
    }
    return 0;
}
