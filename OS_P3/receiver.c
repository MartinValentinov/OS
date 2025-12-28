#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define PIPE_NAME "chat_pipe"
#define BUFFER_SIZE 10

int main() {
    int fd;
    char buffer[BUFFER_SIZE];
    int cur = 0;
    char c;
    ssize_t bytesRead;

    fd = open(PIPE_NAME, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    printf("Receiver: Waiting for messages...\n");

    while (1) {
        cur = 0;

        while (1) {
            bytesRead = read(fd, &c, 1);
            if (bytesRead == -1) {
                perror("read");
                goto cleanup;
            }

            if (bytesRead == 0) {
                printf("Sender disconnected.\n");
                goto cleanup;
            }

            if (c == '\0') {
                buffer[cur] = '\0';
                break;
            }

            if (cur < BUFFER_SIZE - 1) {
                buffer[cur++] = c;
            }
        }

        printf("Friend: %s\n", buffer);

        if (strcmp(buffer, "exit") == 0) {
            break;
        }
    }

cleanup:
    close(fd);
    unlink(PIPE_NAME);
    return 0;
}