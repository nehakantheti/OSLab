#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define BUFFER_SIZE 20
int main() {
    pid_t pid;
    int pipefds[2];
    int returnstatusofpipe;
    char readmsg[BUFFER_SIZE];

    // Creating pipe
    returnstatusofpipe = pipe(pipefds);     
    if (returnstatusofpipe == -1) {
        perror("Pipe creation failed");
        exit(1);
    }

    // Fork only after creating the pipe.
    pid = fork();
    if (pid < 0) {  // Error in fork
        perror("Fork failed");
        exit(1);
    } 
    else if (pid == 0) {  // Child process
        sleep(7);
        printf("In child process\n");
        close(pipefds[1]);  // Close the write end in child
        // 1. If close is commented out, the write end remains open. This can lead to blocking issues 
        // for the child process if it’s waiting for an EOF to finish reading.
        ssize_t bytesRead = read(pipefds[0], readmsg, sizeof(readmsg) - 1);
        if (bytesRead >= 0) {
            // readmsg[bytesRead] = '\0';
            printf("Printing string from pipe - %s\n", readmsg);
        } else {
            perror("Read failed");
        }

        close(pipefds[0]);  // Close the read end
        printf("Child process finished!\n");
    } 
    else {  // Parent process
        close(pipefds[0]);  // Close the read end in parent
        // 1. If close is commented out, the write end of the pipe remains open, which results in a resource leak.
        printf("In parent process\n");
        printf("Enter a string to pass to parent process: ");
        char input[BUFFER_SIZE];
        fgets(input, sizeof(input), stdin);
        // scanf("%s", input);
        // input[strcspn(input, "\n")] = '\0';  // Remove newline character

        ssize_t bytesWritten = write(pipefds[1], input, strlen(input) + 1);
        if (bytesWritten < 0) {
            perror("Write failed");
        } else {
            printf("Parent process - Writing to pipe - %s\nWrite successful\n", input);
        }

        close(pipefds[1]);  // Close the write end
        wait(NULL);
        printf("Finished parent process!\n");
    }

    return 0;
}

// 2. If both ends of the pipe are closed before performing any read or write operations, any subsequent attempts
// to read or write will fail. This results in no data transfer and potential errors & abnormal termination of program.

// 3. If you attempt to both read from and write to the same pipe in the same process, it can lead to deadlocks or
// undefined behavior. The pipe is designed for inter-process communication, and such operations should be confined to separate processes.

// 4. If the parent process writes multiple times to the pipe without the child process reading, the pipe buffer
// will eventually fill up. Once full, the write() calls will block until some data is read from the pipe. 
// If the buffer size is exceeded and no read occurs, further writes will be stalled.

// 5. Synchronization Issues: Without proper synchronization (e.g., using wait()),
//  you may encounter issues where the child or parent might prematurely close or use pipe ends.
// And, In sufficient buffer memory may lead to undefined behaviour or segmentation faults,
// which doesn't allow the proccess to finish read or write processes properly.