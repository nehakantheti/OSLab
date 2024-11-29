// Named pipe sender
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>

int main(){
    int pipefd;
    int msg1, msg2;
    printf("Enter number : ");
    scanf("%d", &msg1);

    char* myfifo = "/tmp/myfifo";
    mkfifo(myfifo, 0666);

    pipefd = open(myfifo, O_WRONLY);
    write(pipefd, &msg1, sizeof(msg1));
    printf("Sender : Number sent is : %d\n", msg1);
    close(pipefd);

    pipefd = open(myfifo, O_RDONLY);
    read(pipefd, &msg2, sizeof(msg2));
    printf("Sender : Reeived negation of the number is : %d\n", msg2);
    close(pipefd);
    return 0;
}