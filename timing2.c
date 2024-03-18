#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* task1(void* arg) {
    printf("Task 1 executed.\n");
    return NULL;
}

void* task2(void* arg) {
    printf("Task 2 executed.\n");
    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    if (pthread_create(&thread1, NULL, &task1, NULL) != 0) {
        fprintf(stderr, "Error creating thread 1\n");
        return 1;
    }

    if (pthread_create(&thread2, NULL, &task2, NULL) != 0) {
        fprintf(stderr, "Error creating thread 2\n");
        return 1;
    }

    // Wait for the created threads to finish their execution.
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}
