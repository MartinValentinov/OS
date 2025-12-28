#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
    int fd[2];
    pipe(fd);

    if (fork() == 0) {
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);
        execlp("ls", "ls", NULL);
        perror("execlp failed");
        exit(EXIT_FAILURE);
    }
    if (fork() == 0) {
        dup2(fd[0], STDIN_FILENO);
        close(fd[1]);
        close(fd[0]);
        execlp("grep", "grep", ".c", NULL);
        perror("execlp failed");
        exit(EXIT_FAILURE);
    }

    close(fd[0]);
    close(fd[1]);

    wait(NULL);
    wait(NULL);

    return 0;
}