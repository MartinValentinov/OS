#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mutex;

void* deposit(void* arg) {
    int* balance = (int*)arg;
    for (int i = 0; i < 50000; i++) {
        pthread_mutex_lock(&mutex);
        *balance += 1;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *withdraw(void* arg) {
    int* balance = (int*)arg;
    for (int i = 0; i < 50000; i++) {
        pthread_mutex_lock(&mutex);
        *balance -= 1;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;
    int balance = 0;

    pthread_mutex_init(&mutex, NULL);
    pthread_create(&t1, NULL, deposit, &balance);
    pthread_create(&t2, NULL, withdraw, &balance);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_mutex_destroy(&mutex);

    printf("Final Balance = %d\n", balance);
    return 0;
}