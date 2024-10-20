// Neha Kantheti
// CS22B1081

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    int pipefd;
    int msg1,msg2;
    printf("Enter Number: ");
    scanf("%d", &msg1);

    char *myfifo = "/tmp/myfifo";

    mkfifo(myfifo, 0666);

    pipefd = open(myfifo, O_WRONLY);
    write(pipefd, &msg1, sizeof(msg1));
    printf("Sender: Sent integer msg1 = %d\n", msg1);
    close(pipefd);

    pipefd = open(myfifo, O_RDONLY);
    read(pipefd, &msg2, sizeof(msg2));
    printf("Sender: Received from receiver (1's compliment): %d\n", msg2);
    close(pipefd);

    return 0;
}
