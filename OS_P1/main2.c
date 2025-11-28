#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

void intToBinary(int n, char *buffer) {
    for (int i = 7; i >= 0; i--) {
        buffer[7 - i] = (n & (1 << i)) ? '1' : '0';
    }
    buffer[8] = '\0';
}

int main() {
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    int len = sizeof(arr) / sizeof(arr[0]);

    int fd = open("dump.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    char bin[9];

    for (int i = 0; i < len; i++) {
        intToBinary(arr[i], bin);

        char formatted[12];
        snprintf(formatted, sizeof(formatted), "%.4s %.4s\n", bin, bin + 4);

        write(fd, formatted, 10);
    }

    close(fd);
    return 0;
}