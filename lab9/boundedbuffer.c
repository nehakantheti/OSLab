// Neha Kantheti
// CS22B1081
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];   // Buffer to hold produced items
int count = 0;             // Number of items in the buffer
sem_t empty;               // Semaphore for tracking empty slots in buffer
sem_t full;                // Semaphore for tracking filled slots in buffer
pthread_mutex_t mutex;     // Mutex to control access to shared resources

int n = 5;
int copy;
// Function to produce an item (returns a simple integer)
int produce_item() {
    static int item = 0;  // Keeps track of item numbers
    return ++item;
}

// Function to consume an item (prints the consumed item)
void consume_item(int item) {
    printf("Consumed item: %d\n", item);
}

// Producer thread function
void* producer(void* arg) {
    while (n) {
        int item = produce_item();  // Produce an item
        
        sem_wait(&empty);           // Wait if there are no empty slots
        pthread_mutex_lock(&mutex);  // Lock mutex for exclusive access to buffer

        // Add the produced item to the buffer
        buffer[count] = item;
        count++;
        printf("Produced item: %d\n", item);

        pthread_mutex_unlock(&mutex); // Unlock mutex after producing
        sem_post(&full);              // Signal that there is a new filled slot
        n--;
        sleep(1);  // Slow down production for demonstration
    }
    return NULL;
}

// Consumer thread function
void* consumer(void* arg) {
    while (copy) {
        sem_wait(&full);            // Wait if there are no filled slots
        pthread_mutex_lock(&mutex);  // Lock mutex for exclusive access to buffer

        // Remove an item from the buffer
        int item = buffer[--count];
        consume_item(item);

        pthread_mutex_unlock(&mutex); // Unlock mutex after consuming
        sem_post(&empty);             // Signal that there is a new empty slot
        copy--;
        sleep(1);  // Slow down consumption for demonstration
    }
    return NULL;
}

int main() {
    printf("No of items: ");
    scanf("%d", &n);
    copy = n;
    pthread_t prod, cons;

    // Initialize the semaphores
    sem_init(&empty, 0, BUFFER_SIZE); // Initial empty slots equal buffer size
    sem_init(&full, 0, 0);            // Initial filled slots are zero
    pthread_mutex_init(&mutex, NULL);  // Initialize the mutex

    // Create producer and consumer threads
    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    // Wait for the threads to finish (they won't, as we have infinite loops)
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    // Destroy semaphores and mutex
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}