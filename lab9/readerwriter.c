// Neha Kantheti
// CS22B1081
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t wrt;                // Semaphore to control writer access
pthread_mutex_t mutex;    // Mutex to control access to readers_count
int readers_count = 0;    // Count of active readers
int shared_data = 0;      // Shared data resource
int num_reads = 5;       // Total number of reads
int num_writes = 2;       // Total number of writes

// Writer function
void* writer(void* arg) {
    for (int i = 0; i < num_writes; i++) {
        // Request exclusive access to shared resource
        sem_wait(&wrt);
        
        // Writing to the shared resource
        shared_data++;
        printf("Writer wrote: %d\n", shared_data);
        
        // Release exclusive access to shared resource
        sem_post(&wrt);
        
        sleep(1);  // Slow down writer for demonstration
    }
    return NULL;
}

// Reader function
void* reader(void* arg) {
    int reader_id = *(int*)arg;
    
    for (int i = 0; i < num_reads; i++) {
        // Lock mutex to modify readers_count
        pthread_mutex_lock(&mutex);
        readers_count++;
        
        // If this is the first reader, it locks the wrt semaphore
        if (readers_count == 1) {
            sem_wait(&wrt);
        }
        
        pthread_mutex_unlock(&mutex);
        
        // Reading from the shared resource
        printf("Reader %d read: %d\n", reader_id, shared_data);
        
        // Lock mutex to modify readers_count
        pthread_mutex_lock(&mutex);
        readers_count--;
        
        // If this is the last reader, it releases the wrt semaphore
        if (readers_count == 0) {
            sem_post(&wrt);
        }
        
        pthread_mutex_unlock(&mutex);
        
        sleep(1);  // Slow down reader for demonstration
    }
    return NULL;
}

int main() {
    pthread_t writer_thread, reader_threads[5];
    int reader_ids[5] = {1, 2, 3, 4, 5};

    // Initialize the semaphores and mutex
    sem_init(&wrt, 0, 1);
    pthread_mutex_init(&mutex, NULL);

    // Create one writer thread
    pthread_create(&writer_thread, NULL, writer, NULL);

    // Create five reader threads
    for (int i = 0; i < 5; i++) {
        pthread_create(&reader_threads[i], NULL, reader, &reader_ids[i]);
    }

    // Wait for the threads to finish
    pthread_join(writer_thread, NULL);
    for (int i = 0; i < 5; i++) {
        pthread_join(reader_threads[i], NULL);
    }

    // Destroy the semaphores and mutex
    sem_destroy(&wrt);
    pthread_mutex_destroy(&mutex);

    return 0;
}
