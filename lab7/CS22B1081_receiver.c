// Neha Kantheti
// CS22B1081

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    int pipefds;
    int msgg1, msgg2;

    char *myfifo = "/tmp/myfifo";

    pipefds = open(myfifo, O_RDONLY);
    read(pipefds, &msgg1, sizeof(msgg1));
    printf("Receiver: Received integer msgg1 = %d\n", msgg1);
    msgg2 = ~msgg1;
    close(pipefds);

    pipefds = open(myfifo, O_WRONLY);
    write(pipefds, &msgg2, sizeof(msgg2));
    printf("Receiver: Sent one's complement msgg2 = %d\n", msgg2);
    close(pipefds);

    return 0;
}