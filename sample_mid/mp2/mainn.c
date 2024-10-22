#include<stdio.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<signal.h>
#include<sys/types.h>
#include<wait.h>
#include<unistd.h>

#define MAX 10

struct result_buffer{
    long msg_type;
    int total;
    int average;
}result;

void sig_handler(){
    printf("CAUGHT SIG_HANDLER\n");
}

int main(){
    int msg_id;
    pid_t pid;

    pid = fork();

    if(pid < 0){
        perror("Error in forking\n");
        exit(1);
    }
    
    if(pid == 0){
        // Child process
        // sleep(2);
        printf("In child process\n");
        signal(SIGUSR1, sig_handler);
        printf("Child process paused, waiting for signal from parent process\n");
        pause();

        printf("In child process again\n");
        execl("./receiver", "receiver", NULL);

        perror("Error in compiling receiver file\n");
        exit(1);
    }else{
        // Parent process
        int sender_pid = fork();
        if(sender_pid < 0){
            printf("Error in forking sender process\n");
            exit(1);
        }else if(sender_pid == 0){
            printf("Sender program executing...\n");
            execl("./sender", "sender", NULL);

            perror("Error in compiling sender file\n");
            exit(1);
        }


        int status;
        waitpid(sender_pid, &status, 0);
        kill(pid, SIGUSR1);
        waitpid(pid, &status, 0);

        // struct result_buffer result;
        key_t key = ftok("msgqueue", 65);
        int msg_id = msgget(key, IPC_CREAT | 0666);


        msgrcv(msg_id, &result, sizeof(result), 2, 0);

        printf("Parent : Received total time from receiver : %d\n", result.total);
        printf("Parent : Received average time from receiver : %d\n", result.average);

        msgctl(msg_id, IPC_RMID, NULL);

        printf("Message queue cleaned up\n");

    }


    return 0;
}