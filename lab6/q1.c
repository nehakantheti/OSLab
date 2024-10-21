// Program to print length difference between messages passed by parent and grandchild and printing in child process.
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/wait.h>

#define maxSize 128

int main(){
    pid_t pid;
    int retPipe1;
    int pipe1[2], pipe2[2];
    char readmsg[maxSize], buffer[maxSize], grandmsg[maxSize];
    // Process should be forked only after pipe is created or else terminal will jump
    if (pipe(pipe1) == -1) {
        perror("Pipe creation failed");
        exit(1);
    }
    pid = fork();

    if(pid < 0){
        perror("Error in forking!\n");
        exit(1);
    }else if(pid == 0){
        // Creating 2nd pipe
        if (pipe(pipe2) == -1) {
            perror("Pipe creation failed");
            exit(1);
        }
        int childPid = fork();
        if(childPid < 0){
            perror("Error in forking grnad child process!");
            exit(1);
        }
        else if(childPid == 0){
            //In grandchild process
            printf("\nGrand Child Process id : %d  Child process id : %d\n", getpid(), getppid());
            close(pipe2[0]);    // Closing read end of pipe
            printf("Enter a message to write into pipe : \n");
            fgets(buffer, maxSize, stdin);
            
            if(write(pipe2[1], buffer, strlen(buffer)) < 0){
                printf("Write failed!\n");
            }else{
                printf("Write successful in grand child\n");
            }
            close(pipe2[1]);
            exit(0);
        }
        else{
            //In child process
            wait(NULL);
            printf("child id : %d  Parent process id : %d\n", getpid(), getppid());
            close(pipe2[1]);    //Closing write end of pipe
            close(pipe1[1]);    //Closing write end of pipe since child process only reads from pipe here
            
            if(read(pipe1[0], readmsg, maxSize) < 0){
                printf("Read failed\n");
            }else{
                printf("Reading from parent process - %s\n", readmsg);
            }

            printf("Reading grand child message from Child : ");
            if(read(pipe2[0], grandmsg, maxSize) < 0){
                printf("Read failed\n");
            }else{
                printf("%s\n", grandmsg);
            }
            close(pipe1[0]);    //Close read end of pipe after read operation
            close(pipe2[0]);

            printf("Message from grand child - %s\n", grandmsg);
            printf("Message from parent - %s\n", readmsg);
            int grandchild_len = strlen(grandmsg);
            int parent_len = strlen(readmsg);
            if (readmsg[parent_len - 1] == '\n') parent_len--;
            if (grandmsg[grandchild_len - 1] == '\n') grandchild_len--;
            printf("Difference = %d\n", abs(grandchild_len - parent_len));
        }
    }else{
        //In parent process
        close(pipe1[0]);    //Close write end of pipe after write operation since parent here is only writing to pipe.
        printf("Enter message to write to pipe (In parent process): ");
        fgets(buffer, sizeof(buffer), stdin);
        if(write(pipe1[1], buffer, strlen(buffer)) < 0){
            printf("Write failed\n");
        }else{
            printf("Successfully written into pipe 1\n");
        }
        close(pipe1[1]);
        wait(NULL);
    }
    return 0;
}