/*
Write a C program that:

The parent process creates two child processes using fork().
The parent process sends an array of process arrival times and burst times to the first child using a pipe.
The first child uses the First-Come, First-Served (FCFS) scheduling algorithm to calculate the completion times for each process, and writes the results to shared memory.
The second child reads the process completion times from shared memory and calculates the average waiting time and turnaround time.
After calculation, the second child sends a signal to the parent, notifying that the computations are done.
The parent process waits for the signal, reads the results, and displays the average waiting and turnaround times.
 */

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/wait.h>
#include<sys/signal.h>
#include<sys/shm.h>
#define SHM_SIZE 10*sizeof(struct process)

struct process{
    int pid;
    int at;
    int bt;
    int ct;
    int tat;
    int wt;
};

int averageTAT = 0, averageWT = 0;


int compare(const void* a, const void *b){
    return ((struct process*)a)->at - ((struct process*)b)->at;
}

void sig_handler(){
    printf("SIGUSR1 caught, continuing to print average times : \n");
}

int main(){

    int n;
    pid_t p1;
    struct process *p;
    int pipefd[2];

    int key = 1081;
    int shmid;
    struct process *shm, *s;
    struct process *processesInChild;
    struct process *shm1, *shm2;
    shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    int child1pid, child2pid;
    int status1, status2;

    if(pipe(pipefd) == -1){
        printf("Error in creating pipe!\n");
        exit(0);
    }

    p1 = fork();
    if(p1 < 0){
        perror("Error in forking!\n");
        exit(1);
    }else if(p1 == 0){
        // Child 1
        signal(SIGUSR1, sig_handler);
        processesInChild = (struct process*)malloc(n * sizeof(struct process));
        printf("____________IN CHILD 1___________\n");
        // sleep(5);
        pause();
        // child1pid = getpid();

        // Retreiving structure values using pipe
        printf("-----IN CHILD 1 AFTER sleep()--------\n");
        close(pipefd[1]);
        

        read(pipefd[0], processesInChild, n*sizeof(struct process));

        for(int i = 0 ; i < n ; i++){
            printf("pid : %d\n", processesInChild[i].pid);
            printf("at : %d\n", processesInChild[i].at);
            printf("bt : %d\n", processesInChild[i].bt);
        }

        //Implement fcfs logic

        qsort(processesInChild, n, sizeof(struct process), compare);
        processesInChild[0].ct = processesInChild[0].at + processesInChild[0].bt;
        processesInChild[0].tat = processesInChild[0].ct - processesInChild[0].at;
        processesInChild[0].wt = processesInChild[0].tat - processesInChild[0].bt;

        for(int i = 1; i < n;i++){
            processesInChild[i].ct = processesInChild[i-1].ct + processesInChild[i].bt;
            processesInChild[i].tat = processesInChild[i].ct - processesInChild[i].at;
            processesInChild[i].wt = processesInChild[i].tat - processesInChild[i].bt;
        }

        for(int i = 0; i < n;i++){
            printf("pid : %d\n", processesInChild[i].pid);
            printf("at : %d\n", processesInChild[i].at);
            printf("bt : %d\n", processesInChild[i].bt);
            printf("ct : %d\n", processesInChild[i].ct);
            printf("tat : %d\n", processesInChild[i].tat);
            printf("wt : %d\n", processesInChild[i].wt);
        }
        close(pipefd[0]);

        shm1 = shmat(shmid, NULL, 0);
        if(shm1 == (void*)-1){
            perror("Error in attaching shared memory in child 1\n");
            exit(1);
        }

        s = shm1;

        // Writing into shared memory
        for(int i = 0; i < n;i++){
            s[i] = processesInChild[i];
        }

        // if(!shmdt(shm1)){
        //     printf("Detached successfully\n");
        // }else{
        //     printf("Error in detaching shared memory.");
        // }
        shmdt(shm1);
        exit(0);
    }else{
        // Parent process
        // signal(SIGUSR1, sig_handler);
        close(pipefd[0]);
        printf("___________IN PARENT PROCESS___________\n");
        printf("Enter number of processes : ");
        scanf("%d", &n);
        p = (struct process*)malloc(10 * sizeof(struct process*));
        for(int i = 0; i < n;i++){
            printf("Enter pid : ");
            scanf("%d", &p[i].pid);
            printf("Enter arrival time : ");
            scanf("%d", &p[i].at);
            printf("Enter burst time : ");
            scanf("%d", &p[i].bt);
        }


        write(pipefd[1], p, n*sizeof(struct process));
        close(pipefd[1]);
        printf("Write of process successful!\n");
        kill(p1, SIGUSR1);


        printf("Waiting for child process to finish!\n");
        // wait(NULL);     // Waiting for process 1
        waitpid(p1, &status1, 0);

        printf("___________IN PARENT PROCESS AGAIN___________\n");
        int p2 = fork();
        
        
        if(p2 < 0){
            perror("Error in creating 2nd child\n");
            exit(1);
        }else if(p2 == 0){
        // Child 2
        // Assuming it is reaching only after executing process 1
        printf("____________IN CHILD 2______________\n");
        // sleep(5);
        shm2 = shmat(shmid, NULL, 0);
        child2pid = getpid();

        s = shm2;

        for(int i = 0; i < n;i++){
            averageTAT += s[i].tat;
            averageWT += s[i].wt;
        }
        averageTAT /= n;
        averageWT /= n;
        printf("Average TAT = %d\n", averageTAT);
        printf("Average WT = %d\n", averageWT);

        // if(!shmdt(shm2)){
        //     printf("Detachment successful\n");
        // }else{
        //     printf("Detachment unsuccessful\n");
        // }

        shmdt(shm2);

        kill(getppid(), SIGUSR1);
        exit(0);

    }else{
        //Parent process
        sleep(5);
        printf("=================IN FINAL PARENT PROCESS=================\n");
        signal(SIGUSR1, sig_handler);
        // wait(NULL);
        waitpid(p2, &status2, 0);
        
        // while(1){
            // Do nothing until
        // }
        shmctl(shmid, IPC_RMID, NULL);
        printf("Average TAT = %d\n", averageTAT);
        printf("Average WT = %d\n", averageWT);
    }
    }
}