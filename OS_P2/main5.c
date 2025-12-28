#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mutex;
long long total_sum = 0;

typedef struct {
    int* arr;
    int start;
    int end;
} ThreadData;

void* sum_array(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    long long local_sum = 0;

    for (int i = data->start; i < data->end; i++) {
        local_sum += data->arr[i];
    }

    pthread_mutex_lock(&mutex);
    total_sum += local_sum;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    pthread_t t1, t2;
    int n;

    do {
        printf("Enter a positive integer for array size: ");
        scanf("%d", &n);
    } while (n <= 0);

    int arr[n];
    for (int i = 0; i < n; i++) {
        arr[i] = i + 1;
    }

    pthread_mutex_init(&mutex, NULL);

    ThreadData d1 = { arr, 0, n / 2 };
    ThreadData d2 = { arr, n / 2, n };

    pthread_create(&t1, NULL, sum_array, &d1);
    pthread_create(&t2, NULL, sum_array, &d2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&mutex);

    printf("Total Sum = %lld\n", total_sum);
    return 0;
}