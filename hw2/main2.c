#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

#define MAX_INPUT 1024
#define MAX_ARGS 100
#define MAX_COMMANDS 10

int helper(char* str, char* split_by, char* result[], int limit) {

    char* token = strtok(str, split_by);
        
    int i = 0;
    while (token != NULL && i < limit) {
        result[i ++] = token;
        token = strtok(NULL, split_by);
    }
    result[i] = NULL;

    return i;
}

int main() {

    char input[MAX_INPUT];

    while (1) {
        
        printf("shell>");
        fflush(stdout);

        if (fgets(input, MAX_INPUT, stdin) == NULL) {
            printf("\n");
            break;
        }

        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "exit") == 0) {
            break;
        }

        char* commands[MAX_COMMANDS];
        int com_count = helper(input, "|", commands, MAX_COMMANDS);
        if (com_count == 0) continue;

        int fd[2 * (com_count - 1)];
        for (int i = 0; i < com_count - 1; i ++) {
            if (pipe(fd + i * 2) < 0) {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }
        
        for (int i = 0; i < com_count; i ++) {
            char* input_args[MAX_ARGS];
            helper(commands[i], " ", input_args, MAX_ARGS);

            if (fork() == 0) {
                if (i > 0) {
                    if (dup2(fd[(i - 1) * 2], STDIN_FILENO) < 0) {
                        perror("dup2 failed!");
                        exit(EXIT_FAILURE);
                    }
                }
                if (i < com_count - 1) {
                    if (dup2(fd[(i * 2) + 1], STDOUT_FILENO) < 0) {
                        perror("dup2 failed!");
                        exit(EXIT_FAILURE);
                    }
                }

                for (int j = 0; j < (com_count - 1) * 2; j ++) {
                    close(fd[j]);
                }

                if (execvp(input_args[0], input_args) < 0) {
                    perror("execvp failed!");
                    exit(EXIT_FAILURE);
                }
            }
        }
        for (int i = 0; i < (com_count - 1) * 2; i ++) {
            close(fd[i]);
        }
        
        for (int i = 0; i < com_count; i ++) {
            wait(NULL);
        }
    }

    return 0;
}