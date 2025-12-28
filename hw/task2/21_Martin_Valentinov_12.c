#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char* argv[]) {

    //code

    if (argc < 2) {
        fprintf(stderr, "No file given!\n");
        return 1;
    }

    for (int i = 1; i < argc; i ++) {
        int file = open(argv[i], O_RDONLY);
        if (file == -1) {
            perror("File not found!");
            exit(EXIT_FAILURE);
        }

        int lineCount = 1;
        int skippedLines = 0;
        char curCharacter;
        size_t seen;

        while((seen = read(file, &curCharacter, sizeof(char))) > 0) {
            if (curCharacter == '\n') {
                lineCount ++;
            }
        }

        if (seen == -1) {
            perror("Unexpected error while reading the file!");
            exit(EXIT_FAILURE);
        }

        if (lineCount > 10) skippedLines = lineCount - 10;

        if (write(1, argv[i], strlen(argv[i])) == -1) {
            perror("Write of a file name has failed!");
            exit(EXIT_FAILURE);
        }
        if (write(1, "\n", 1) == -1) {
            perror("Write of a new line has failed!");
            exit(EXIT_FAILURE);
        }

        lseek(file, 0, SEEK_SET);
        while((seen = read(file, &curCharacter, sizeof(char))) > 0) {
            if (curCharacter == '\n') {
                skippedLines --;
            }

            if (skippedLines > 0) continue;

            if (write(1, &curCharacter, 1) == -1) {
                perror("Error while writing lines!");
                exit(EXIT_FAILURE);
            }
        }

        if (seen == -1) {
            perror("Unexpected error while reading the file!");
            exit(EXIT_FAILURE);
        }

        if (write(1, "\n\n", 2) == -1) {
            perror("Write of a new line has failed!");
            exit(EXIT_FAILURE);
        }

        if (close(file) == -1) {
            perror("Unexpected error when closing the file!");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
