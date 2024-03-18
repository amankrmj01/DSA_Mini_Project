#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

void* delay_and_execute(void* arg) {
    int delay_in_seconds = *((int*)arg);
    sleep(delay_in_seconds);

    // Your task here.
    printf("Delayed task executed.\n");

    return NULL;
}

void immediate_task() {
    // Another task that can be executed immediately.
    printf("Immediate task executed.\n");
}

int main() {
    pthread_t thread;
    int delay_in_minutes = 5;
    int delay_in_seconds = delay_in_minutes * 3;

    if (pthread_create(&thread, NULL, &delay_and_execute, &delay_in_seconds) != 0) {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }


    // Wait for the created thread to finish its execution.
    pthread_join(thread, NULL);

    // The main thread can continue its execution here without being blocked.
    immediate_task();

    
    return 0;
}
