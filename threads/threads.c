// CS22B1081
// Neha Kantheti
#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 5
#define NUM_COUNT 1000
#define CHUNK_SIZE (NUM_COUNT / NUM_THREADS)

long thread_sums[NUM_THREADS];

// Thread function to calculate the sum of a portion of numbers
void* sum_partial(void* arg) {
    long thread_id = (long)arg;
    long start = thread_id * CHUNK_SIZE + 1;
    long end = (thread_id + 1) * CHUNK_SIZE;
    long local_sum = 0;

    // Calculate the sum for the thread's range
    for (long i = start; i <= end; i++) {
        local_sum += i;
    }

    // Store the result in the corresponding thread_sums entry
    thread_sums[thread_id] = local_sum;
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    long total_sum = 0;

    // Create threads
    for (long i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, sum_partial, (void*)i);
    }

    // Wait for all threads to finish
    for (long i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Calculate the total sum by combining the results from all threads
    for (long i = 0; i < NUM_THREADS; i++) {
        total_sum += thread_sums[i];
    }

    // Print the final sum after all threads have finished
    printf("The sum of the first %d numbers is: %ld\n", NUM_COUNT, total_sum);

    return 0;
}