#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/wait.h>
#include<sys/signal.h>
#include<sys/shm.h>
#include<string.h>
#define SHM_SIZE 10*sizeof(struct process)

int n;

struct process{
    int pid;
    int at;
    int bt;
    int ct;
    int tat;
    int wt;
};

void sig_handler(){
    printf("Caught SIGUSR1\n");
}

int compare(const void *a, const void *b){
    return (*(struct process*)a).at - (*(struct process*)b).at;
}

int main(){
    pid_t p1, p2;
    int pipefd[2];  // Pipes are unidirectional, This is for parent writing into it & child reading from it
    int pipefd1[2];     // This is for child writing into it & parent reading from it.
    int shmid;
    int status1, status2;

    printf("Enter n : ");
    scanf("%d", &n);

    if(pipe(pipefd) == -1){
        perror("Error in creating pipe\n");
        exit(1);
    }

    if(pipe(pipefd1) == -1){
        perror("Error in creting 2nd pipe\n");
        exit(1);
    }

    key_t key = 1081;
    shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);

    p1 = fork();
    if(p1 < 0){
        perror("Error in forking\n");
        exit(1);
    }else if(p1 == 0){
        // Child 1
        signal(SIGUSR1, sig_handler);
        pause();
        
        struct process* shm1,*s;
        // Attaching shared memory
        if((shm1 = shmat(shmid, NULL, 0)) < 0){
            perror("Error in attaching shared mem\n");
        }

        close(pipefd[1]);
        struct process processesInChild[n];
        read(pipefd[0], processesInChild, n*sizeof(struct process));
        printf("N = %d\n", n);
        for(int i = 0 ; i < n ; i++){
            printf("pid : %d\n", processesInChild[i].pid);
            printf("at : %d\n", processesInChild[i].at);
            printf("bt : %d\n", processesInChild[i].bt);
        }

        //Implement FCFS logic

        qsort(processesInChild, n, sizeof(struct process), compare);
        processesInChild[0].ct = processesInChild[0].at + processesInChild[0].bt;
        processesInChild[0].tat = processesInChild[0].ct - processesInChild[0].at;
        processesInChild[0].wt = processesInChild[0].tat - processesInChild[0].bt;

        for(int i = 1; i < n;i++){
            processesInChild[i].ct = processesInChild[i-1].ct + processesInChild[i].bt;
            processesInChild[i].tat = processesInChild[i].ct - processesInChild[i].at;
            processesInChild[i].wt = processesInChild[i].tat - processesInChild[i].bt;
        }

        printf("PID\t AT\t BT\t CT\t TAT\t WT\n");
        for(int i = 0; i < n;i++){
            printf(" %d\t", processesInChild[i].pid);
            printf(" %d\t", processesInChild[i].at);
            printf(" %d\t", processesInChild[i].bt);
            printf(" %d\t", processesInChild[i].ct);
            printf(" %d\t", processesInChild[i].tat);
            printf(" %d\n", processesInChild[i].wt);
        }

        // s = shm1;
        // shm1 = processesInChild;

        memcpy(shm1, processesInChild, sizeof(processesInChild));

        // Detaching Shared mem
        shmdt(shm1);
        exit(0);
    }else{
        // Parent process
        printf("In parent process : \n");
        
        printf("N = %d\n", n);
        struct process p[n];

        for(int i = 0; i < n;i++){
            printf("Enter pid : ");
            scanf("%d", &p[i].pid);
            printf("Enter arrival time : ");
            scanf("%d", &p[i].at);
            printf("Enter burst time : ");
            scanf("%d", &p[i].bt);
        }

        close(pipefd[0]);   // Closing read end of pipe

        write(pipefd[1], p, sizeof(p));

        close(pipefd[1]);   // Closing write end of pipe

        kill(p1, SIGUSR1);

        waitpid(p1, &status1, 0);
        // Finishes implementation of FCFS and returns to parent process

        p2 = fork();

        if(p2 < 0){
            perror("Error in forking 2nd child\n");
            exit(1);
        }else if(p2 == 0){
            // Child 2
            // Read from shared mem

            // Attaching shared mem
            printf("In child 2\n");

            struct process *shm2, *sm;
            struct process *child2processes;
            // child2processes = (struct process*)malloc(n*sizeof(struct process));
            printf("CHECK 1\n");
            if((shm2 = shmat(shmid, NULL, 0)) < 0){
                perror("Error in attaching in 2nd child\n");
                exit(1);
            }

            printf("CHECK 2\n");

            // sm = shm2;
            child2processes = shm2;

            printf("CHECK 3\n");

            double avgTAT = 0, avgWT = 0;
            int sumTAT  = 0, sumWT = 0;
            for(int i = 0; i < n;i++){
                // printf("I = %d\n", i);
                printf("PID - %d\n", child2processes[i].pid);
                // printf("%d %d\n", child2processes[i].tat, child2processes[i].wt);
                sumTAT += child2processes[i].tat;
                sumWT += child2processes[i].wt;
            }

            printf("CHECK 4\n");
            printf("N i s : %d\n", n);

            printf(" AVGTAT = %d & AVGWT = %d\n", sumTAT, sumWT);

            avgTAT = (double)sumTAT/(double)n;
            avgWT = (double)sumWT/(double)n;
-
            printf(" AVGTAT = %f & AVGWT = %f\n", avgTAT, avgWT);

            close(pipefd1[0]);   // Closing read end of pipe

            write(pipefd1[1], &avgTAT, sizeof(double));
            printf("TAT Write successful\n");

            write(pipefd1[1], &avgWT, sizeof(double));
            printf("WT Write successful\n");

            close(pipefd1[1]);

            kill(getpid(), SIGUSR1);

            // Detaching shared mem
            shmdt(shm2);
            exit(0);

        }else{
            // Parent process
            printf("In parent process before child2\n");
            waitpid(p2, &status2, 0);
            // Waits for 2nd child
            printf("In parent process after child2\n");

            close(pipefd1[1]);

            double tat_avg, wt_avg;
            read(pipefd1[0], &tat_avg, sizeof(double));
            read(pipefd1[0], &wt_avg, sizeof(double));

            printf("Average Waiting time = %lf\n", tat_avg);
            printf("Average Waiting time = %lf\n", wt_avg);
            close(pipefd1[0]);

            shmctl(shmid, IPC_RMID, NULL);

            printf("Finished successfully\n");
        }
    }

    return 0;
}