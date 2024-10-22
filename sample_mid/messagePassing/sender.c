// sender.c
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>

#define MAX 10

struct msg_buffer {
    long msg_type;
    int burst_times[MAX];
} message;

int main() {
    key_t key;
    int msgid;

    // Generate a unique key for message queue
    key = ftok("msgqueue", 65);

    // Create message queue
    msgid = msgget(key, 0666 | IPC_CREAT);

    // Message type
    message.msg_type = 1;

    // Array of burst times to send (for example, 5 burst times)
    printf("Enter the number of burst times: ");
    int n;
    scanf("%d", &n);

    printf("Enter %d burst times:\n", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &message.burst_times[i]);
    }

    // Send burst times to the message queue
    msgsnd(msgid, &message, sizeof(message), 0);

    printf("Sender: Burst times sent to message queue.\n");

    return 0;
}
