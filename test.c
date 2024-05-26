#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

int main(void)
{
    int error;
    execve("/usr/bin/notexist", NULL, NULL);
    printf("%s\n", strerror(error));
    perror("notexist");
    return (0);
}