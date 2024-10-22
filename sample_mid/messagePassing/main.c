// parent_child.c
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

struct result_buffer {
    long msg_type;
    int total;
    int average;
};

int main() {
    pid_t pid;
    int msgid;

    // Fork the process
    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    }

    if (pid == 0) {
        // Child process: Execute receiver
        execl("./receiver", "receiver", NULL);
        // If execl fails
        perror("Exec failed");
        exit(1);
    } else {
        // Parent process: Execute sender
        execl("./sender", "sender", NULL);
        // If execl fails
        perror("Exec failed");
        exit(1);

        // Wait for child process to complete
        // wait(NULL);
        waitpid(pid, NULL, 0);

        // Parent reads the result from the message queue
        key_t key;
        struct result_buffer result;

        // Generate the same key
        key = ftok("msgqueue", 65);

        // Access the message queue
        msgid = msgget(key, 0666 | IPC_CREAT);

        // Receive result from the message queue
        ssize_t x = msgrcv(msgid, &result, sizeof(result), 2, 0);
        if(x < 0){
            perror("ERRORRRRRR\n");
        }

        printf("Parent: Received total burst time: %.2f\n", result.total);
        printf("Parent: Received average burst time: %.2f\n", result.average);

        // Clean up the message queue
        msgctl(msgid, IPC_RMID, NULL);

        printf("Message queue cleaned up.\n");
    }

    return 0;
}
