#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>

typedef struct {
    int count;
    pthread_mutex_t mutex;
} SharedResource;

void initSharedResource(SharedResource* resource) {
    resource->count = 0;
    pthread_mutex_init(&resource->mutex, NULL);
}

void increment(SharedResource* resource) {
    pthread_mutex_lock(&resource->mutex);
    resource->count++;
    pthread_mutex_unlock(&resource->mutex);
}

void decrement(SharedResource* resource) {
    pthread_mutex_lock(&resource->mutex);
    resource->count--;
    pthread_mutex_unlock(&resource->mutex);
}

int getCount(SharedResource* resource) {
    int count;
    pthread_mutex_lock(&resource->mutex);
    count = resource->count;
    pthread_mutex_unlock(&resource->mutex);
    return count;
}

typedef struct {
    SharedResource* sharedResource;
    char name[10];
} Gate;

void checkIn(Gate* gate) {
    increment(gate->sharedResource);
    printf("Check in from %s. Total number is: %d\n", gate->name, getCount(gate->sharedResource));
}

void checkOut(Gate* gate) {
    if (getCount(gate->sharedResource) > 0) {
        decrement(gate->sharedResource);
        printf("Check out from %s. Total number is: %d\n", gate->name, getCount(gate->sharedResource));
    }
}

void* gateRoutine(void* arg) {
    Gate* gate = (Gate*)arg;
    while (true) {
        srand(time(NULL));
        int delayMilliseconds = rand() % 10 + 1000;
        usleep(delayMilliseconds * 1000);
        bool outOperation = rand() % 2 == 1;
        if (outOperation) {
            checkOut(gate);
        } else {
            checkIn(gate);
        }
    }
    return NULL;
}

int main() {
    SharedResource sharedResource;
    initSharedResource(&sharedResource);

    pthread_t thread1, thread2;
    Gate gate1 = {&sharedResource, "East"};
    Gate gate2 = {&sharedResource, "West"};

    pthread_create(&thread1, NULL, gateRoutine, (void*)&gate1);
    pthread_create(&thread2, NULL, gateRoutine, (void*)&gate2);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    pthread_mutex_destroy(&sharedResource.mutex);
    return 0;
}