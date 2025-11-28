#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>

typedef struct
{
    char data;
    unsigned char nextElementAddress;
} block;

int main(int argc, char* argv[]){

    // code

    if (argc < 2) {
        fprintf(stderr, "No file given!\n");
        exit(EXIT_FAILURE);
    }

    int file = open(argv[1], O_RDONLY);
    if (file == -1) {
        perror("File not found!");
        exit(EXIT_FAILURE);
    }

    block current;

    while (1) {
        size_t seen = read(file, &current, sizeof(block));
        if (seen == -1) {
            perror("Unexpected error when reading the file!");
            exit(EXIT_FAILURE);
        }

        if (seen == 0) {
            fprintf(stderr, "File empty!");
            exit(EXIT_FAILURE);
        }

        if (write(1, &current.data, 1) != 1) {
            perror("Unexpected error when writing!");
            exit(EXIT_FAILURE);
        }

        if (current.nextElementAddress == 0) break;

        if (lseek(file, current.nextElementAddress, SEEK_SET) == -1) {
            perror("Unexpected error when changing cursor position!");
            exit(EXIT_FAILURE);
        }
    }

    close(file);
    if (file == -1) {
        perror("Unexpected error when closing the file!");
        exit(EXIT_FAILURE);
    }

    return 0;
}