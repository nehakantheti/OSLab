//CS22B1081
//Neha Kantheti
#include <stdio.h>
#include <stdlib.h>

int current_time = 0;


typedef struct {
    int pid;
    int arrival_time;
    int priority;
    int burst_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
    int response_time;
} Process;

int compare(const void *a, const void *b) {
    Process *p1 = (Process *)a;
    Process *p2 = (Process *)b;
    
    if (p1->priority == p2->priority)
        return p1->arrival_time - p2->arrival_time;
    return p1->priority - p2->priority;
}

void priority_scheduling(Process *processes, int n) {
    float avg_tat = 0, avg_wt = 0, avg_rt = 0;
    
    for (int i = 0; i < n; i++) {
        qsort(processes, n, sizeof(Process), compare);
        
        int idx = -1;
        for (int j = 0; j < n; j++) {
            if (processes[j].arrival_time <= current_time && processes[j].completion_time == 0) {
                idx = j;
                break;
            }
        }
        
        if (idx == -1) {
            current_time++;
            i--;
            continue;
        }
        
        processes[idx].completion_time = current_time + processes[idx].burst_time;
        processes[idx].turnaround_time = processes[idx].completion_time - processes[idx].arrival_time;
        processes[idx].waiting_time = processes[idx].turnaround_time - processes[idx].burst_time;
        processes[idx].response_time = processes[idx].waiting_time;
        
        current_time += processes[idx].burst_time;

        avg_tat += processes[idx].turnaround_time;
        avg_wt += processes[idx].waiting_time;
        avg_rt += processes[idx].response_time;
    }
    
    avg_tat /= n;
    avg_wt /= n;
    avg_rt /= n;
    
    printf("P_ID\tAT\tPR\tBT\tFT\tTAT\tWT\tRT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", processes[i].pid, processes[i].arrival_time, processes[i].priority, processes[i].burst_time,
               processes[i].completion_time, processes[i].turnaround_time, processes[i].waiting_time, processes[i].response_time);
    }
    
    printf("\nAverage Turnaround Time: %.2f\n", avg_tat);
    printf("Average Waiting Time: %.2f\n", avg_wt);
    printf("Average Response Time: %.2f\n", avg_rt);
}

int main() {
    int n;
    printf("Enter n : ");
    scanf("%d", &n);
    int s = 3;
    Process *processes = (Process *)malloc(3 * sizeof(Process));
    while (s < n) {
        s *= 2;
        Process *new_processes = (Process *)realloc(processes, s * sizeof(Process));
        if (new_processes == NULL) {
            free(processes);
            return 1;
        }
        processes = new_processes;
    }

    int p_id[n];
    int arrival_time[n];
    int priority[n];
    int burst_time[n];

    for(int i = 0;i< n;i++){
        scanf("%d", &p_id[i]);
        scanf("%d", &arrival_time[i]);
        scanf("%d", &priority[i]);
        scanf("%d", &burst_time[i]);

    }
    
    
    for (int i = 0; i < n; i++) {
        processes[i].pid = p_id[i];
        processes[i].arrival_time = arrival_time[i];
        processes[i].priority = priority[i];
        processes[i].burst_time = burst_time[i];
        processes[i].completion_time = 0;
    }
    
    priority_scheduling(processes, n);
    
    free(processes);
    
    return 0;
}