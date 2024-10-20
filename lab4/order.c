#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>

int main(){

    pid_t process1, process2, childPID1, childPID2;
    process1 = fork();
    int status1, status2;

    if(process1 < 0){       // Error!
        printf("Error!");
    }else if(process1 > 0){ // Parent process
        childPID1 = wait(&status1);
        
        process2 = fork();
        
        childPID2 = wait(&status2);
        
    
        if(process2 == 0){      //Child 2 process
            printf("Child 1 My id is %d and my parent id is %d. Chennai\n",getpid(), getppid());
        }else{          //Parent Process continution.
            printf("Parent Process my id is %d and my parent id is %d. India\n", getpid(), getppid());

        }
    }else{            //Child 1 Process
        int pid3, pid4;
        if((pid3 = fork()) && (pid4 = fork())){
            wait(NULL);
            printf("Child 2 My id is %d and my parent id is %d. Kandigai\n",getpid(), getppid());
        }else{
            if(pid3 == 0){
                // sleep(0.5);
                printf("Child 3 My id is %d and my parent id is %d.  Your Name and Roll-No, CSE/ CSE-AI Department\n", getpid(), getppid());
            }
            if(pid4 == 0){
                printf("Child 4 My id is %d and my parent id is %d.  IIITDM Kancheepuram\n",getpid(), getppid());
            }
        }

    }
    
    return 0;
}