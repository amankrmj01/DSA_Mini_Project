#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int shared_value = 0;

void* change_value_after_delay(void* arg) {
    int delay_in_seconds = *((int*)arg);
    sleep(delay_in_seconds);

    // Change the shared value here.
    shared_value = 10;
    printf("Shared value changed to %d.\n", shared_value);

    return NULL;
}

void immediate_task() {
    // Another task that can be executed immediately.
    printf("Immediate task executed.\n");
}

int main() {
    pthread_t thread;
    int delay_in_seconds = 60;  // 1 minute
    printf("im here\n");
    if (pthread_create(&thread, NULL, &change_value_after_delay, &delay_in_seconds) != 0) {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }
    printf("im here\n");

    // The main thread can continue its execution here without being blocked.
    immediate_task();

    // Wait for the created thread to finish its execution.
    pthread_join(thread, NULL);

    return 0;
}
