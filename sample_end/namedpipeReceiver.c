// Named pipe receiver
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<unistd.h>

int main(){
    int pipefd;
    int msg1, msg2;
    char* myfifo = "/tmp/myfifo";

    mkfifo(myfifo, 0666);

    pipefd = open(myfifo, O_RDONLY);
    read(pipefd, &msg1, sizeof(msg1));
    printf("Receiver : Message received from Sender is : %d\n", msg1);
    close(pipefd);

    msg2 = ~msg1;
    pipefd = open(myfifo, O_WRONLY);
    write(pipefd, &msg2, sizeof(msg2));
    printf("Receiver : Negation of received number is : %d\n", msg2);
    close(pipefd);
    return 0;
}