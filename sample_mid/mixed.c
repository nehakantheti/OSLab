#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/shm.h>
#include <string.h>

#define SHM_SIZE 1024  // Shared memory size

int shm_id;  // Shared memory ID
key_t key = 1234;  // Key for shared memory

// Signal handler for the parent to catch signal from the second child
void handle_sigusr1(int sig) {
    printf("Parent: Received signal from second child that computations are done.\n");
}

int main() {
    int pipefd[2];  // Pipe file descriptors
    pid_t p1, p2;  // Process IDs for the two children

    // Array for arrival and burst times (for example purposes)
    int arrival_times[] = {0, 2, 4, 6};
    int burst_times[] = {5, 3, 8, 6};
    int n = sizeof(arrival_times) / sizeof(arrival_times[0]);

    // Create shared memory
    if ((shm_id = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    // Create the pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(1);
    }

    // Fork the first child
    if ((p1 = fork()) == 0) {
        // First child process: FCFS scheduling

        close(pipefd[1]);  // Close writing end of the pipe

        // Read arrival and burst times from the pipe
        int arrival[10], burst[10];
        read(pipefd[0], arrival, sizeof(arrival_times));
        read(pipefd[0], burst, sizeof(burst_times));
        close(pipefd[0]);

        // FCFS Scheduling to calculate completion times
        int completion[10], current_time = 0;
        for (int i = 0; i < n; i++) {
            if (arrival[i] > current_time) {
                current_time = arrival[i];
            }
            current_time += burst[i];
            completion[i] = current_time;
        }

        // Write completion times to shared memory
        int *shm_ptr = (int *)shmat(shm_id, NULL, 0);
        memcpy(shm_ptr, completion, sizeof(completion));
        shmdt(shm_ptr);

        exit(0);  // First child exits
    } else if (p1 < 0) {
        perror("Fork failed for first child");
        exit(1);
    }

    // Fork the second child
    if ((p2 = fork()) == 0) {
        // Second child process: Calculate waiting and turnaround times

        // Read completion times from shared memory
        int *shm_ptr = (int *)shmat(shm_id, NULL, 0);
        int completion[10];
        memcpy(completion, shm_ptr, n * sizeof(int));
        shmdt(shm_ptr);

        // Calculate waiting and turnaround times
        int waiting_times[10], turnaround_times[10];
        int total_waiting = 0, total_turnaround = 0;
        for (int i = 0; i < n; i++) {
            turnaround_times[i] = completion[i] - arrival_times[i];
            waiting_times[i] = turnaround_times[i] - burst_times[i];
            total_waiting += waiting_times[i];
            total_turnaround += turnaround_times[i];
        }

        float avg_waiting = (float)total_waiting / n;
        float avg_turnaround = (float)total_turnaround / n;

        // Write the results to shared memory
        shm_ptr = (int *)shmat(shm_id, NULL, 0);
        shm_ptr[n] = total_waiting;  // Writing total waiting time at index n
        shm_ptr[n + 1] = total_turnaround;  // Writing total turnaround time at index n+1
        shmdt(shm_ptr);

        // Send signal to parent
        kill(getppid(), SIGUSR1);

        exit(0);  // Second child exits
    } else if (p2 < 0) {
        perror("Fork failed for second child");
        exit(1);
    }

    // Parent process: Send data to the first child and handle signal

    // Register the signal handler for SIGUSR1
    signal(SIGUSR1, handle_sigusr1);

    close(pipefd[0]);  // Close reading end of the pipe

    // Write arrival and burst times to the pipe
    write(pipefd[1], arrival_times, sizeof(arrival_times));
    write(pipefd[1], burst_times, sizeof(burst_times));
    close(pipefd[1]);

    // Wait for the signal from the second child
    pause();

    // Read the results from shared memory
    int *shm_ptr = (int *)shmat(shm_id, NULL, 0);
    int total_waiting = shm_ptr[n];  // Reading total waiting time from shared memory
    int total_turnaround = shm_ptr[n + 1];  // Reading total turnaround time from shared memory
    shmdt(shm_ptr);

    // Display the results
    float avg_waiting = (float)total_waiting / n;
    float avg_turnaround = (float)total_turnaround / n;
    printf("Parent: Average waiting time: %.2f\n", avg_waiting);
    printf("Parent: Average turnaround time: %.2f\n", avg_turnaround);

    // Cleanup: remove shared memory
    shmctl(shm_id, IPC_RMID, NULL);

    // Wait for child processes to complete
    wait(NULL);
    wait(NULL);

    return 0;
}
