#include <stdio.h>
#include <stdbool.h>

#define PROCESSES 6
#define RESOURCES 4

// Function to check if the system is in a safe state
bool isSafe(int allocation[PROCESSES][RESOURCES], int max[PROCESSES][RESOURCES], int available[RESOURCES], int need[PROCESSES][RESOURCES]) {
    int work[RESOURCES];
    bool finish[PROCESSES] = {false};
    int safeSequence[PROCESSES];
    int count = 0;

    // Copy available resources to work
    for (int i = 0; i < RESOURCES; i++) {
        work[i] = available[i];
    }

    while (count < PROCESSES) {
        bool found = false;
        for (int p = 0; p < PROCESSES; p++) {
            if (!finish[p]) {
                bool canProceed = true;
                for (int r = 0; r < RESOURCES; r++) {
                    if (need[p][r] > work[r]) {
                        canProceed = false;
                        break;
                    }
                }
                if (canProceed) {
                    for (int r = 0; r < RESOURCES; r++) {
                        work[r] += allocation[p][r];
                    }
                    safeSequence[count++] = p;
                    finish[p] = true;
                    found = true;
                }
            }
        }
        if (!found) {
            printf("UNSAFE\n");
            return false;
        }
    }

    printf("SAFE\nSafe sequence: ");
    for (int i = 0; i < PROCESSES; i++) {
        printf("P%d ", safeSequence[i]);
    }
    printf("\n");
    return true;
}

int main() {
    int allocation[PROCESSES][RESOURCES] = {
        {0, 1, 0, 3},
        {2, 0, 0, 1},
        {3, 0, 2, 0},
        {2, 1, 1, 0},
        {0, 0, 0, 2},
        {1, 3, 1, 0}
    };

    int max[PROCESSES][RESOURCES] = {
        {6, 4, 3, 4},
        {2, 2, 2, 2},
        {4, 3, 3, 2},
        {3, 3, 2, 2},
        {4, 2, 2, 3},
        {3, 3, 2, 4}
    };

    int available[RESOURCES] = {3, 2, 1, 2};

    // Calculate the need matrix
    int need[PROCESSES][RESOURCES];
    for (int i = 0; i < PROCESSES; i++) {
        for (int j = 0; j < RESOURCES; j++) {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }

    // Check initial state safety
    isSafe(allocation, max, available, need);

    // User input loop for additional requests
    while (1) {
        int process;
        int request[RESOURCES];
        printf("Enter the process number for an additional request (0-5): ");
        scanf("%d", &process);
        printf("Enter the request for P%d: ", process);
        for (int i = 0; i < RESOURCES; i++) {
            scanf("%d", &request[i]);
        }

        // Check if the request can be granted
        bool validRequest = true;
        for (int i = 0; i < RESOURCES; i++) {
            if (request[i] > need[process][i] || request[i] > available[i]) {
                validRequest = false;
                break;
            }
        }

        if (validRequest) {
            // Temporarily allocate resources to check for safety
            for (int i = 0; i < RESOURCES; i++) {
                available[i] -= request[i];
                allocation[process][i] += request[i];
                need[process][i] -= request[i];
            }

            if (isSafe(allocation, max, available, need)) {
                printf("The request was granted.\n");
            } else {
                printf("UNSAFE. The request was discarded.\n");
                for (int i = 0; i < RESOURCES; i++) {
                    available[i] += request[i];
                    allocation[process][i] -= request[i];
                    need[process][i] += request[i];
                }
            }
        } else {
            printf("UNSAFE. The request is not valid and was discarded.\n");
        }

        printf("Do you want to enter another request? (y/n): ");
        char cont;
        scanf(" %c", &cont);
        if (cont != 'y') break;
    }

    return 0;
}
