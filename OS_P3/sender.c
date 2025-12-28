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

    if (mkfifo(PIPE_NAME, 0666) == -1) {
        perror("mkfifo");
    }

    fd = open(PIPE_NAME, O_WRONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    printf("Sender: Type your messages below (type 'exit' to quit):\n");
    while (1) {
        if (write(STDOUT_FILENO, "You: ", 5) == -1) {
            perror("write");
            break;
        }
        cur = 0;
        while(1){
            bytesRead = read(STDIN_FILENO, &c, 1);
            if (bytesRead == -1) {
                perror("read");
                break;
            }
            if (c == '\n'){
                buffer[cur] = '\0';
                break;
            }
            else{
                buffer[cur] = c;
            }
            if ((cur + bytesRead)<BUFFER_SIZE){
                cur += bytesRead;
            }
        }

        if (write(fd, buffer, strlen(buffer) + 1) == -1) {
            perror("write");
            break;
        }

        if (strcmp(buffer, "exit") == 0) {
            break;
        }

    }

    close(fd);
    unlink(PIPE_NAME);
    return 0;
}