#include<stdio.h>
#include<stdlib.h>

float avgWT = 0, avgTAT = 0;

struct node{
    int pid;
    int at;
    int bt;
    int ct;
    int tat, wt, rt;
    struct node* next;
};

struct node* createNode(int pid, int at, int bt){
    // printf("Creating Node!");
    struct node* temp = (struct node*)malloc(sizeof(struct node));
    temp->pid = pid;
    temp->at = at;
    temp->bt = bt;
    temp->next = NULL;
    return temp;
}

struct node* insertNode(struct node* head, struct node* front, struct node* rear, struct node* temp){
    printf("Insertinggg\n");
    if(head == NULL || head->at > temp->at || (head->at == temp->at && head->pid > temp->pid)){
        printf("Head updated!");
        temp->next = head;
        head = temp;
    }else{
        struct node* ptr = head;
            while(ptr->next != NULL && (ptr->next->at < temp->at || (ptr->next->at == temp->at && ptr->next->pid < temp->pid))){
                ptr = ptr->next;
            }
            temp->next = ptr->next;
            ptr->next = temp;
    }
    front = head;
    rear = temp;
    printf("New Node Inserted successfully!\n");
    return head;
}

void print(struct node* head){
    printf("FCFS\n");
    struct node* temp = head;
    printf("ProcessID  AT  BT  FT  TAT  WT  RT\n");
    while(temp != NULL){
        printf("%d          %d   %d   %d   %d   %d   %d\n", temp->pid, temp->at, temp->bt, temp->ct, temp->tat, temp->wt, temp->rt);
        temp = temp->next;
    }
    printf("Average Waiting Time : %f \n", avgWT);
    printf("Average Turn Around Time : %f \n", avgTAT);
}

struct node* deleteNode(struct node* head){
    printf("%d ", head->pid);
    struct node* ch = head;
    head = head->next;
    return head;
}

void calculate(struct node* head){
    printf("\n\n\nSchedule : ");
    struct node* temp = head;
    int total = 0;
    int count = 0;
    while(head != NULL){
        count++;
        head->ct = total + head->bt;
        total = head->ct;
        head->tat = head->ct-head->at;
        head->wt = head->tat - head->bt;
        head->rt = head->wt;
        avgWT += head->wt;
        avgTAT += head->tat;
        head = deleteNode(head);
    }
    avgTAT/=count;
    avgWT/=count;
    printf("\n");
}

int main(){
    int numOfProcesses;
    printf("Enter number of processes : ");
    scanf("%d", &numOfProcesses);
    
    struct node* head = NULL;
    struct node* front = head;
    struct node* rear = head;
    
    for(int i = 0;i< numOfProcesses;i++){
        int pid, at, bt;
        printf("Enter PID : ");
        scanf("%d", &pid);
        printf("Enter Arrival Time : ");
        scanf("%d", &at);
        printf("Enter Burst Time : ");
        scanf("%d", &bt);
        struct node* newNode = createNode(pid, at, bt);
        head = insertNode(head, front, rear, newNode);
    }
    
    struct node* dummy = head;
    
    if(head == NULL){
        printf("Head is NULL\n");
    }
    
    // print(head);
    
    calculate(head);
    
    print(dummy);
    
    return 0;
}