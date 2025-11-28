#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
		return EXIT_FAILURE;
	}

	int file = open(argv[1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (file == -1) {
		perror("Error opening file");
		return EXIT_FAILURE;
	}

    size_t bytesWritten = 0;
    for (int i = 2; i < argc; i++) {
        bytesWritten = write(file, argv[i], strlen(argv[i]));
        bytesWritten = write(file, " ", 1);
        if (bytesWritten == -1) {
            perror("Error writing to file");
            close(file);
            return EXIT_FAILURE;
        }
    }

    if (bytesWritten == -1) {
        perror("Error writing to file");
        close(file);
        return EXIT_FAILURE;
    }
	
	close(file);

	return 0;
}
