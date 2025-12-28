#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if (pid > 0) {
        printf("Parent PID: %d, child PID: %d\n", getpid(), pid);
        printf("Sleeping for 30 seconds...\n");
        sleep(30);
    } else if (pid == 0) {
        printf("Child PID: %d exiting\n", getpid());
        exit(0);
    } else {
        perror("fork failed");
    }

    return 0;
}