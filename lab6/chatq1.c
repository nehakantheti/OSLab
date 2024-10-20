#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define maxSize 128

int main() {
    pid_t pid;
    int pipe1[2], pipe2[2];  // pipe1: parent -> child, pipe2: grandchild -> child
    char readmsg[maxSize], buffer[maxSize], grandmsg[maxSize];

    // Create pipes
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("Pipe creation failed");
        exit(1);
    }

    // Fork to create child process
    pid = fork();

    if (pid < 0) {
        perror("Error in forking child process!");
        exit(1);
    } else if (pid == 0) {
        // In child process
        pid_t childPid = fork();  // Fork to create grandchild process

        if (childPid < 0) {
            perror("Error in forking grandchild process!");
            exit(1);
        } else if (childPid == 0) {
            // In grandchild process
            printf("\nGrandchild Process ID: %d, Child Process ID: %d\n", getpid(), getppid());
            close(pipe2[0]);  // Close read end of pipe2 (grandchild -> child)
            printf("Enter a message from grandchild: ");
            fgets(buffer, maxSize, stdin);

            // Write to pipe2 (grandchild -> child)
            if (write(pipe2[1], buffer, strlen(buffer)) < 0) {
                perror("Write to pipe2 failed");
            } else {
                printf("Message sent from grandchild to child.\n");
            }
            close(pipe2[1]);  // Close write end of pipe2
            exit(0);
        } else {
            // In child process
            wait(NULL);  // Wait for grandchild to finish
            close(pipe2[1]);  // Close write end of pipe2 (grandchild writes)
            close(pipe1[1]);  // Close write end of pipe1 (parent writes)

            // Read message from parent
            if (read(pipe1[0], readmsg, maxSize) <= 0) {
                perror("Read from parent failed");
            } else {
                printf("Message from parent: %s", readmsg);
            }
            close(pipe1[0]);  // Close read end of pipe1

            // Read message from grandchild
            if (read(pipe2[0], grandmsg, maxSize) <= 0) {
                perror("Read from grandchild failed");
            } else {
                printf("Message from grandchild: %s", grandmsg);
            }
            close(pipe2[0]);  // Close read end of pipe2

            // Print the length difference between messages
            int parent_len = strlen(readmsg);
            int grandchild_len = strlen(grandmsg);
            
            // Remove newline characters from message lengths
            if (readmsg[parent_len - 1] == '\n') parent_len--;
            if (grandmsg[grandchild_len - 1] == '\n') grandchild_len--;
            
            printf("Difference in length between parent and grandchild messages: %d\n", abs(parent_len - grandchild_len));
        }
    } else {
        // In parent process
        close(pipe1[0]);  // Close read end of pipe1 (parent writes)

        // Input message from parent
        printf("Enter a message from parent: ");
        fgets(buffer, maxSize, stdin);

        // Write to pipe1 (parent -> child)
        if (write(pipe1[1], buffer, strlen(buffer)) < 0) {
            perror("Write to pipe1 failed");
        } else {
            printf("Message sent from parent to child.\n");
        }
        close(pipe1[1]);  // Close write end of pipe1

        // Wait for the child to finish
        wait(NULL);
    }

    return 0;
}
