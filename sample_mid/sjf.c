// K Bala Sai Manvitha - CS22B1030
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Process{
    int pid;
    int AT;
    int BT;
    int CT;
    int TAT;
    int WT;
    int RT;
    int completed;
};

// Function to select the process with the shortest job at the current time
struct Process* select_shortest_job(struct Process *proc, int n, int current_time) {
    struct Process *current = NULL;
    for(int i=0; i<n; i++) {
        if(!proc[i].completed && proc[i].AT <= current_time) {
            if(current == NULL || proc[i].BT < current->BT) {
                current = &proc[i];
            }
        }
    }
    return current;
}

// Function to get Priority order via given priority or if priority is same then by Arrival time.
int burstCompare(const void *a, const void *b)
{
    struct Process *p1 = (struct Process*)a;
    struct Process *p2 = (struct Process*)b;
    if(p1->AT == p2->AT)
        return p1->BT - p2->BT;
    else
        return p1->AT - p2->AT;
}

// Function to find the Schedule and finding Avg values
void schedule(struct Process *proc, int n)
{
    int current_time = 0;
    float tot_WT = 0, tot_TAT = 0, tot_RT = 0;
    int remProcess = n;
    printf("P_ID\tAT\tBT\tCT\tTAT\tWT\tRT\n");
    while(remProcess > 0)
    {
        struct Process *current = select_shortest_job(proc, n, current_time);
        
        // If no process is ready at the current time, increment time
        if(current == NULL) {                            
            current_time++;
            continue;
        }
        // Calculate times for the selected process
        current->CT = current_time + current->BT;
        current->TAT = current->CT - current->AT;
        current->WT = current->TAT - current->BT;
        current->RT = current->WT;
        
        current_time = current->CT;
        current->completed = 1;
        tot_WT += current->WT;
        tot_TAT += current->TAT;
        tot_RT += current->RT;

        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n", current->pid, current->AT, current->BT, current->CT, current->TAT, current->WT, current->RT);
        remProcess--;
    }
    printf("Average Waiting Time: %f \n", tot_WT/n);
    printf("Average Response Time: %f \n", tot_RT/n);
    printf("Average Turnaround Time: %f \n", tot_TAT/n);
}

int main()
{
    int n;
    printf("Enter total no of Processes: ");
    scanf("%d", &n);
    struct Process *process = (struct Process*) malloc(n * sizeof(struct Process));

    for(int i=0; i<n; i++) {
        printf("Enter pID, Arrival time, Burst Time: \n");
        scanf("%d %d %d", &process[i].pid, &process[i].AT, &process[i].BT);
        process[i].completed = 0;  // Mark all processes as not completed
    }

    qsort(process, n, sizeof(struct Process), burstCompare);    // Sorting the priority orders
    schedule(process, n);
    free(process); 
    return 0;
}