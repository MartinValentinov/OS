#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
		return EXIT_FAILURE;
	}

	int file = open(argv[1], O_RDONLY);
	if (file == -1) {
		perror("Error opening file");
		return EXIT_FAILURE;
	}

	char buffer[256];
	size_t bytesRead;
	while ((bytesRead = read(file, buffer, sizeof(buffer) - 1)) > 0) {
		buffer[bytesRead] = '\0';
		printf("%s", buffer);
	}
	printf("\n");
	
	close(file);

	return 0;
}
