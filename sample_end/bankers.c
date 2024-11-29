#include <stdio.h>
#include <stdbool.h>

#define P 5  // Number of processes
#define R 3  // Number of resources

// Function to calculate whether the system is in a safe state or not
bool isSafe(int available[], int max[][R], int allocation[][R], int need[][R]) {
    int work[R];
    bool finish[P] = {false};
    int safeSeq[P];
    int count = 0;

    // Initialize work as available resources
    for (int i = 0; i < R; i++) {
        work[i] = available[i];
    }

    // Try to find a safe sequence
    while (count < P) {
        bool found = false;
        for (int p = 0; p < P; p++) {
            if (!finish[p]) {
                // Check if the process needs can be met with available resources
                bool canAllocate = true;
                for (int i = 0; i < R; i++) {
                    if (need[p][i] > work[i]) {
                        canAllocate = false;
                        break;
                    }
                }

                // If can allocate, simulate allocation
                if (canAllocate) {
                    for (int i = 0; i < R; i++) {
                        work[i] += allocation[p][i];  // Release resources
                    }
                    finish[p] = true;
                    safeSeq[count++] = p;  // Add to safe sequence
                    found = true;
                    break;
                }
            }
        }

        // If no process can be allocated, system is not in a safe state
        if (!found) {
            return false;
        }
    }

    // If we have a safe sequence
    printf("Safe Sequence: ");
    for (int i = 0; i < P; i++) {
        printf("P%d ", safeSeq[i]);
    }
    printf("\n");

    return true;
}

int main() {
    // Example values for resources, maximum demand, allocation and need
    int available[] = {3, 3, 2}; // Available resources
    int max[P][R] = {
        {7, 5, 3},
        {3, 2, 2},
        {9, 0, 2},
        {2, 2, 2},
        {4, 3, 3}
    }; // Maximum resources needed by each process

    int allocation[P][R] = {
        {0, 1, 0},
        {2, 0, 0},
        {3, 0, 2},
        {2, 1, 1},
        {0, 0, 2}
    }; // Currently allocated resources to each process

    // Calculate the need matrix (max - allocation)
    int need[P][R];
    for (int i = 0; i < P; i++) {
        for (int j = 0; j < R; j++) {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }

    // Check for system safety
    if (!isSafe(available, max, allocation, need)) {
        printf("System is not in a safe state.\n");
    }

    return 0;
}