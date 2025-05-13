#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
int main()
{
    pid_t my_pid;
    my_pid = getpid();
    printf("MY process ID is: %d\n", my_pid);
    return 0;
}