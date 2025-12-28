#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

// Structure to hold thread data
struct ThreadData {
    int start_row;
    int end_row;
    int **A;
    int **B;
    int **C;
    int N;
    int P;
    int M;
};

// Function to allocate a 2D array
int **allocate_matrix(int rows, int cols) {
    int **matrix = malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++) {
        matrix[i] = malloc(cols * sizeof(int));
    }
    return matrix;
}

// Function to free a 2D array
void free_matrix(int **matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

// Function to read a matrix from a file
int **read_matrix(const char *filename, int *rows, int *cols) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char ch;
    char buffer[16];
    int buffer_index = 0;
    *rows = 0;
    *cols = 0;

    while (read(fd, &ch, 1) > 0 && ch != '\n') {
        if (ch == ';') {
            buffer[buffer_index] = '\0';
            *rows = atoi(buffer);
            buffer_index = 0;
        } else {
            buffer[buffer_index++] = ch;
        }
    }
    buffer[buffer_index] = '\0';
    *cols = atoi(buffer);

    int **matrix = allocate_matrix(*rows, *cols);

    int row = 0, col = 0;
    buffer_index = 0;
    while (read(fd, &ch, 1) > 0) {
        if (ch == ';' || ch == '\n') {
            buffer[buffer_index] = '\0';
            matrix[row][col] = atoi(buffer);
            buffer_index = 0;
            if (ch == ';') {
                col++;
            } else if (ch == '\n') {
                col = 0;
                row++;
            }
        } else {
            buffer[buffer_index++] = ch;
        }
    }
    buffer[buffer_index] = '\0';
    matrix[row][col] = atoi(buffer);

    close(fd);
    return matrix;
}

// Function to write a matrix to a file
void write_matrix(const char *filename, int **matrix, int rows, int cols) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char dimension_buffer[32];
    int dim_len = snprintf(dimension_buffer, sizeof(dimension_buffer), "%d;%d\n", rows, cols);
    if (write(fd, dimension_buffer, dim_len) == -1) {
        perror("Error writing dimensions to file");
        close(fd);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char num_buffer[16];
            int num_len = snprintf(num_buffer, sizeof(num_buffer), "%d", matrix[i][j]);

            if (write(fd, num_buffer, num_len) == -1) {
                perror("Error writing to file");
                close(fd);
                exit(EXIT_FAILURE);
            }

            if (j < cols - 1) {
                if (write(fd, ";", 1) == -1) {
                    perror("Error writing to file");
                    close(fd);
                    exit(EXIT_FAILURE);
                }
            }
        }

        if (write(fd, "\n", 1) == -1) {
            perror("Error writing to file");
            close(fd);
            exit(EXIT_FAILURE);
        }
    }

    close(fd);
}

void* multiply_matrix(void* args) {
    struct ThreadData* data = (struct ThreadData*) args;

    for (int i = data->start_row; i < data->end_row; i ++) {
        for (int j = 0; j < data->P; j ++) {
            data->C[i][j] = 0;
            for (int k = 0; k < data->N; k ++) {
                data->C[i][j] += data->A[i][k] * data->B[k][j];
            }
        }
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Usage: %s <matrix_a.txt> <matrix_b.txt> <matrix_c.txt> <num_threads>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *file_a = argv[1];
    const char *file_b = argv[2];
    const char *file_c = argv[3];
    int num_threads = atoi(argv[4]);

    if (num_threads <= 0) {
        printf("Number of threads must be greater than 0\n");
        exit(EXIT_FAILURE);
    }

    int M, N, N1, N2, P;
    int **A = read_matrix(file_a, &M, &N1);
    int **B = read_matrix(file_b, &N2, &P);

    int **C = allocate_matrix(M, P);

    printf("Read matrices from files.\n");


    //START HERE --------------------------------------------------------------
    
    if (N1 != N2) {
        fprintf(stderr, "Invalid matrices");
        exit(EXIT_FAILURE);
    }

    pthread_t thrs[num_threads];
    struct ThreadData thrs_data[num_threads];

    int num_rows_per_thread = M / num_threads;

    for (int i = 0; i < num_threads; i ++) {
        thrs_data[i].start_row = i * num_rows_per_thread;
        if (i == num_threads - 1) {
            thrs_data[i].end_row = M;
        } else {
            thrs_data[i].end_row = (i + 1) * num_rows_per_thread;
        }

        thrs_data[i].A = A;
        thrs_data[i].B = B;
        thrs_data[i].C = C;
        thrs_data[i].N = N2;
        thrs_data[i].P = P;

        pthread_create(&thrs[i], NULL, multiply_matrix, &thrs_data[i]);
    }

    for (int i = 0; i < num_threads; i ++) {
        pthread_join(thrs[i], NULL);
    }
    
    printf("Calculated multiplication.\n");
    //END HERE --------------------------------------------------------------


    write_matrix(file_c, C, M, P);
    printf("Written result to file.\n");

    free_matrix(A, M);
    free_matrix(B, N2);
    free_matrix(C, M);

    return 0;
}