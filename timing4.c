#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_AMBULANCES 5

int available_ambulances = NUM_AMBULANCES;
pthread_mutex_t lock;

void* return_ambulance(void* arg) {
    sleep(60);
    pthread_mutex_lock(&lock);
    available_ambulances++;
    printf("An ambulance has returned and is now available.\n");
    pthread_mutex_unlock(&lock);
    return NULL;
}

void call_ambulance() {
    pthread_t thread;
    pthread_mutex_lock(&lock);
    if (available_ambulances > 0) {
        available_ambulances--;
        printf("Ambulance dispatched! %d ambulance(s) remaining.\n", available_ambulances);
        pthread_create(&thread, NULL, &return_ambulance, NULL);
        pthread_detach(thread);  // Don't need to join this thread later.
    } else {
        printf("No ambulances available. Please wait.\n");
    }
    pthread_mutex_unlock(&lock);
}

int main() {
    int choice;

    pthread_mutex_init(&lock, NULL);

    while (1) {
        printf("\n1. Call ambulance\n2. Exit\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                call_ambulance();
                break;
            case 2:
                printf("Exiting.\n");
                pthread_mutex_destroy(&lock);
                return 0;
            default:
                printf("Invalid choice. Please enter 1 or 2.\n");
                break;
        }
    }
}
