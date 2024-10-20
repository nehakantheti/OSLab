#include<stdio.h>
#include<stdlib.h>

int quantumTime,n,CS=0,a[10]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},currentProcessTime=0;
float arrivalTime, total_wt=0,total_rt=0,total_turnAroundTime=0;

struct process
{
    int processId;
    int arrivalTime;
    int burstTime;
    int rt;
    struct process*left;
    struct process*right;
};

struct process*createProcess(int processId,int arrivalTime,int burstTime);

void insertIntoReadyQueue(struct process**head,int processId,int arrivalTime,int burstTime);

struct process* removeFromReadyQueue(struct process**head);

int main()
{
    struct process* head=NULL;
    printf("Enter the number of processes:");
    scanf("%d",&n);
    int processId[n],arrivalTime[n],burstTime[n];
    printf("Enter the time quantum:");
    scanf("%d",&quantumTime);
    for(int i=0;i<n;i++)
    scanf("%d %d %d",&processId[i],&arrivalTime[i],&burstTime[i]);
    
    int i=0,turnAroundTime=0,waitingTime=0;
    
    while(i<n || head!=NULL)
    {
        if(head!=NULL)
        {
            currentProcessTime++;
            struct process*temp1= removeFromReadyQueue(&head);
            
            if(a[temp1->processId-1]==-1)
            a[temp1->processId-1]=currentProcessTime-temp1->arrivalTime;
            
            if(temp1->burstTime<=quantumTime)
            {
                CS++;
                currentProcessTime+=temp1->burstTime;
                while(i<n && arrivalTime[i]<=currentProcessTime)
                {
                    insertIntoReadyQueue(&head,processId[i],arrivalTime[i],burstTime[i]);
                    i++;
                }
                turnAroundTime=currentProcessTime-temp1->arrivalTime;
                waitingTime=turnAroundTime-burstTime[temp1->processId-1];
                total_turnAroundTime+=turnAroundTime;
                total_wt+=waitingTime;
                printf("%d %d %d %d %d %d %d\n",temp1->processId,temp1->arrivalTime,burstTime[temp1->processId-1],currentProcessTime,turnAroundTime,waitingTime,a[temp1->processId-1]);
            }
            else
            {
                if(a[temp1->processId]==-1)
                a[temp1->processId-1]=currentProcessTime-temp1->arrivalTime;
                CS++;
                currentProcessTime+=quantumTime;
                temp1->burstTime=temp1->burstTime-quantumTime;
                while(i<n && arrivalTime[i]<=currentProcessTime)
                {
                    insertIntoReadyQueue(&head,processId[i],arrivalTime[i],burstTime[i]);
                    i++;
                }
                insertIntoReadyQueue(&head,temp1->processId,temp1->arrivalTime,temp1->burstTime);
            }
            
        }
        else
        {
            if(arrivalTime[i]>currentProcessTime)
            {
            CS++;
            currentProcessTime=arrivalTime[i];
            }
            insertIntoReadyQueue(&head,processId[i],arrivalTime[i],burstTime[i]);
            i++;
            
        }
    }
    float f1;
    f1=(float)(currentProcessTime-(CS+1))/currentProcessTime;
    printf("Number of context switches: %d\n",CS+1);
    printf("CPU utilization: %.2f\n",f1*100);
    printf("Average turn around time: %.2f\n",total_turnAroundTime/n);
    printf("Average waiting time: %.2f\n",total_wt/n);
    
    return 0;
    
}

struct process*createProcess(int processId,int arrivalTime,int burstTime)
{
    struct process*temp=(struct process*)malloc(sizeof(struct process));
    temp->processId=processId;
    temp->arrivalTime=arrivalTime;
    temp->burstTime=burstTime;
    temp->rt=-1;
    temp->left=NULL;
    temp->right=NULL;
    return temp;
}

void insertIntoReadyQueue(struct process**head,int processId,int arrivalTime,int burstTime)
{
    struct process*temp=createProcess(processId,arrivalTime,burstTime);
    if(*head==NULL)
    *head=temp;
    else
    {
        struct process*current=*head;
        while(current->right!=NULL)
        current=current->right;
        current->right=temp;
        temp->left=current;
    }
}

struct process* removeFromReadyQueue(struct process**head)
{
    if(*head==NULL)
    {
        printf("linked list is empty");
        return NULL;
    }
    else
    {
        struct process*temp=*head;
        (*head)=(*head)->right;
        if(*head!=NULL)
        (*head)->left=NULL;
        return temp;
    }
}
