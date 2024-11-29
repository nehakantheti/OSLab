// Signals
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<sys/wait.h>

void handle_sigfpe(){
    printf("Floating point exception caught\n");
    // kill(getpid(), SIGKILL);
    exit(0);
}

void handle_sigint(){
    printf("SIGINT(Ctrl + C) caught, terminating\n");
}

void handle_sigusr1(){
    printf("SIGUSR caught\n");
}

int main(){
    pid_t p1, p2;
    printf("Parent Started...\n");
    p1 = fork();

    if(p1 < 0){
        printf("Error in forking child, terminating with exit status 1...\n");
        exit(1);
    }else if(p1 == 0){  // Child process
        printf("Child process started\n");
        p2 = fork();    // Forking grand child process
        if(p2 < 0){
            printf("Error in forking grand child process, terminating with exit status 1...\n");
            exit(1);
        }else if(p2 == 0){  // Grand child process
            signal(SIGFPE, handle_sigfpe);
            printf("grand child process started\n");
            int x = 0;
            int y = 2 / x;
            printf("THIS SHOULD NOT PRINT\n");

        }else{             // Child process continues
            wait(NULL);
            signal(SIGINT, handle_sigint);
            printf("Grand child process completed\n");
            printf("Child process continuing...\n");
            printf("Child : Waiting for SIGINT(Ctrl + C)...\n");

            // while(1){
            //     sleep(0.1);
            // }
            raise(SIGINT);
        }
    }else{              // Parent process continues
        wait(NULL);
        printf("Parent process continuing...\n");
        signal(SIGUSR1, handle_sigusr1);
        printf("Parent : Child completed\n");
        raise(SIGUSR1);
        printf("Parent completed\n");
    }
    return 0;
}