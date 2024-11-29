#include <stdio.h>
#include <stdlib.h>

#define MAX 100

// Structure to represent a process
struct Process {
    int pid;      // Process ID
    int burst;    // Burst time (execution time)
    int wait;     // Waiting time
    int turnAround;  // Turnaround time
};

// Comparison function for sorting processes by burst time
int compareBurstTime(const void *a, const void *b) {
    struct Process *processA = (struct Process *)a;
    struct Process *processB = (struct Process *)b;

    return processA->burst - processB->burst;  // Compare burst times
}

// Function to calculate waiting time and turnaround time for all processes
void findWaitingAndTurnaroundTimes(struct Process p[], int n) {
    p[0].wait = 0;  // The first process has 0 waiting time

    // Calculate waiting time for each process
    for (int i = 1; i < n; i++) {
        p[i].wait = p[i - 1].wait + p[i - 1].burst;  // Waiting time of a process = sum of burst times of previous processes
    }

    // Calculate turnaround time for each process
    for (int i = 0; i < n; i++) {
        p[i].turnAround = p[i].burst + p[i].wait;  // Turnaround time = burst time + waiting time
    }
}

// Function to calculate and print average waiting time and average turnaround time
void calculateAverageTimes(struct Process p[], int n) {
    int totalWait = 0, totalTurnAround = 0;
    printf("\nProcess\tBurst Time\tWaiting Time\tTurnaround Time\n");

    for (int i = 0; i < n; i++) {
        totalWait += p[i].wait;
        totalTurnAround += p[i].turnAround;
        printf("P%d\t\t%d\t\t%d\t\t%d\n", p[i].pid, p[i].burst, p[i].wait, p[i].turnAround);
    }

    printf("\nAverage Waiting Time: %.2f", (float)totalWait / n);
    printf("\nAverage Turnaround Time: %.2f", (float)totalTurnAround / n);
}

int main() {
    int n;

    // Input the number of processes
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    struct Process p[n];

    // Input the burst times for each process
    printf("\nEnter the burst times for each process:\n");
    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;  // Process ID (starting from 1)
        printf("Process P%d: ", p[i].pid);
        scanf("%d", &p[i].burst);
    }

    // Sort processes by burst time using qsort()
    qsort(p, n, sizeof(struct Process), compareBurstTime);

    // Calculate waiting time and turnaround time
    findWaitingAndTurnaroundTimes(p, n);

    // Calculate and print average waiting time and average turnaround time
    calculateAverageTimes(p, n);

    return 0;
}