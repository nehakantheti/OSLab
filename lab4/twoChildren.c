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
        // childPID1 = wait(&status1);
        // childPID1 = wait(NULL);
        childPID1 = waitpid(process1, &status1, 0);
        
        process2 = fork();
        
        // childPID2 = wait(&status2);
        // childPID2 = wait(NULL);
        childPID1 = waitpid(process2, &status2, 0);

        
    
        if(process2 == 0){      //Child 2 process
            printf("value returned by fork() (child 2)= %d\n", process2);
            printf("In Child 2 process - Process id = %d, Parent id = %d\n", getpid(), getppid());
            char* args[] = {"./Program33.c",NULL};
            // execvp(args[0], args);
            // execl("./program33",NULL);
            execlp("./program33", NULL);
        }else{


            printf("In parent process!\n");

            printf("IIITDM Kancheepuram !");

            printf("In Parent process - Process id = %d, Parent id = %d\n", getpid(), getppid());
            printf("Terminated child processes - Child Process1 id = %d, Child Process2 id = %d\n", childPID1, childPID2);
        }
    }else{            //Child 1 Process
        printf("value returned by fork() = %d (child 1)\n", process1);
        printf("In Child 1 process - Process id = %d, Parent id = %d\n", getpid(), getppid());
        char* args[] = {"./Program32.c",NULL};
        // execvp(args[0], args);
        // execl("./program32",NULL);
        execlp("./program33", NULL);

    }

    return 0;
}