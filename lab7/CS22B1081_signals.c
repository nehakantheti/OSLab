// Neha Kantheti
// CS22B1081

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void handle_sigfpe(int sig) {
    printf("Grandchild: Caught SIGFPE, terminating using SIGKILL.\n");
    kill(getpid(), SIGKILL);
}

void handle_sigint(int sig) {
    printf("\nChild: Caught SIGINT (Ctrl+C), terminating.\n");
    exit(0);
}

void handle_sigusr1(int sig) {
    printf("Parent: Caught SIGUSR1.\n");
}

int main() 
{
    pid_t p1, p2;
    printf("Parent: Started.\n");
    p1 = fork();

    if(p1 < 0){
        perror("Fork creation failed!!\n");
        return -1;
    }

    // Child process
    else if(p1 == 0){
        p2 = fork();
        // Grand Child Process
        if (p2 == 0){
            printf("Grandchild: Started.\n");
            signal(SIGFPE, handle_sigfpe);      // Registering the signal for floating point exceptions
            int x = 0;
            int y = 1 / x; 
            printf("Grandchild: This will not print due to SIGFPE.\n");
        } 
        else {
            wait(NULL);
            printf("Child: Started.\n");
            // signal(SIGINT, SIG_IGN); 
            signal(SIGINT, handle_sigint);          // Registering signal for cntrl C termination
            printf("Child: Grandchild completed.\n");
            printf("Child: Waiting for SIGINT (Ctrl+C).\n");

            // Keep the child process running until SIGINT is caught
            while(1) {
                sleep(5);
            }
        }
    } 
    else {
        // Parent process waits for the child to complete
        wait(NULL);
        // signal(SIGINT, SIG_IGN); 
        signal(SIGUSR1, handle_sigusr1);
        printf("Parent: Child completed.\n");
        printf("Parent: Waiting for SIGUSR1.\n");
        raise(SIGUSR1);
        printf("Parent Completed!!\n");
    }

    return 0;
}
