// Abhishek M J
// CS21B2018

#include <stdio.h>
#include <stdlib.h>

struct Process
{
    int pid;
    int at;
    int bt;
    int ft, tat, wt, rt;
    int start_time;
    int run_time;
    struct Process *next;
};

struct Process *head = NULL;
int n;

struct Process *copyProcessSruct(struct Process *p)
{
    struct Process *new_p = (struct Process *)malloc(sizeof(struct Process));
    new_p->pid = p->pid;
    new_p->at = p->at;
    new_p->bt = p->bt;
    new_p->next = NULL;
    return new_p;
}

void swapNodes(struct Process *a, struct Process *b)
{
    struct Process *temp = copyProcessSruct(a);
    a->pid = b->pid;
    a->at = b->at;
    a->bt = b->bt;

    b->pid = temp->pid;
    b->at = temp->at;
    b->bt = temp->bt;
}

void heapify(struct Process *p)
{
    struct Process *min = p;
    struct Process *l = p->next;
    struct Process *r;
    if (l)
        r = l->next;
    else
        r = l;

    if (l != NULL && l->bt < min->bt)
    {
        min = l;
    }
    if (r != NULL && r->bt < min->bt)
    {
        min = r;
    }

    if (min != p)
    {
        swapNodes(p, min);
        heapify(min);
    }
}

void insertReadyQueue(struct Process *p)
{
    struct Process *new_p = copyProcessSruct(p);
    if (head == NULL)
    {
        head = copyProcessSruct(new_p);
    }
    else
    {
        new_p->next = head;
        head = new_p;

        struct Process *temp = head;
        while (temp != NULL)
        {
            heapify(temp);
            temp = temp->next;
        }
    }
}

struct Process *popReadyQueue()
{
    if (head == NULL)
    {
        return NULL;
    }
    else
    {
        struct Process *min = copyProcessSruct(head);
        head = head->next;

        struct Process *temp = head;
        while (temp != NULL)
        {
            heapify(temp);
            temp = temp->next;
        }

        return min;
    }
}

struct Process *getProcessAt(struct Process* processes, int time)
{
    struct Process *result = NULL;
    for (int i = 0; i < n; i++)
    {
        if (processes[i].at == time)
        {
            if (result == NULL)
            {
                result = copyProcessSruct(&processes[i]);
            }
            else
            {
                struct Process *temp = result;
                while (temp->next != NULL)
                {
                    temp = temp->next;
                }
                temp->next = copyProcessSruct(&processes[i]);
            }
        }
    }
    return result;
}

void sjf(struct Process *processes)
{
    printf("\nSJF\n");
    printf("P_id:  AT\tBT\tFT\tTAT\tWT\tRT\n");

    float total_wt = 0, total_tat = 0;
    int total_completed = 0;
    int is_running = 0;
    struct Process *p = NULL;
    for (int time = 0; total_completed != n; time++)
    {
        struct Process *arrving_processes = getProcessAt(processes, time);
        while (arrving_processes != NULL)
        {
            insertReadyQueue(arrving_processes);
            arrving_processes = arrving_processes->next;
        }

        if (is_running)
        {
            if (p->run_time == p->bt)
            {
                is_running = 0;
                total_completed++;

                p->ft = time;
                p->tat = p->ft - p->at;
                p->wt = p->tat - p->bt;
                p->rt = p->start_time - p->at;
                
                printf("   %d:  %d\t%d\t%d\t%d\t%d\t%d\n", p->pid, p->at, p->bt, p->ft, p->tat, p->wt, p->rt);
                
                total_wt += p->wt;
                total_tat += p->tat;
            }
            else
            {
                p->run_time++;
                continue;
            }
        }
        if (!is_running)
        {
            p = popReadyQueue();
            if (p == NULL)
            {
                continue;
            }
            p->start_time = time;
            is_running = 1;
            p->run_time++;
        }
    }
    
    printf("Avg waiting time: %f\n", total_wt/n);
    printf("Avg turnaround time: %f\n", total_tat/n);
}

int main()
{
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    
    printf("Enter Arrival time and Burst time:\n");
    struct Process processes[n];
    for (int i = 0; i < n; i++)
    {
        struct Process p;
        p.pid = i+1;
        p.run_time = 0;
        p.next = NULL;
        
        printf("For Process_id %d: ", p.pid);
        scanf("%d %d", &p.at, &p.bt);
        
        processes[i] = p;
    }
    
    sjf(processes);
    
    return 0;
}