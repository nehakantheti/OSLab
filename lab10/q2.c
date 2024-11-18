#include <stdio.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <stdlib.h>

#define NUM_THREADS 5

// Thread function to simulate workload
void* thread_function(void* arg) {
    int thread_id = *(int*)arg;
    printf("Thread %d: Starting execution\n", thread_id);
    // Simulate workload with sleep to observe scheduling order
    sleep(1);
    printf("Thread %d: Finished execution\n", thread_id);
    pthread_exit(NULL);
}

void set_thread_priority(pthread_attr_t *attr, int priority, int policy) {
    struct sched_param param;
    param.sched_priority = priority;
    pthread_attr_setschedpolicy(attr, policy);
    pthread_attr_setschedparam(attr, &param);
}

int main() {
    pthread_t threads[NUM_THREADS];
    pthread_attr_t attr;
    int thread_ids[NUM_THREADS];
    int policy;

    // Initialize thread attributes
    pthread_attr_init(&attr);

    // Set scheduling policy to FIFO (First-In, First-Out)
    policy = SCHED_FIFO;
    printf("Scheduling policy set to FIFO\n");

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        int priority = 10 + i; // Assign different priorities

        // Set thread scheduling parameters
        set_thread_priority(&attr, priority, policy);

        // Create thread with specific attributes
        if (pthread_create(&threads[i], &attr, thread_function, &thread_ids[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    // Wait for all threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Set scheduling policy to Round-Robin
    policy = SCHED_RR;
    printf("\nScheduling policy set to Round-Robin\n");

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        int priority = 10 + i; // Assign different priorities

        // Set thread scheduling parameters
        set_thread_priority(&attr, priority, policy);

        // Create thread with specific attributes
        if (pthread_create(&threads[i], &attr, thread_function, &thread_ids[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    // Wait for all threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    policy = SCHED_OTHER; 
    printf("\nScheduling policy set to SCHED_OTHER(priority)\n");

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        int priority = 10 + i; // Assign different priorities

        // Set thread scheduling parameters
        set_thread_priority(&attr, priority, policy);

        // Create thread with specific attributes
        if (pthread_create(&threads[i], &attr, thread_function, &thread_ids[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    // Wait for all threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    

    // Destroy thread attributes
    pthread_attr_destroy(&attr);

    return 0;
}