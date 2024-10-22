// receiver.c
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>

#define MAX 10

struct msg_buffer {
    long msg_type;
    int burst_times[MAX];
} message;

struct result_buffer {
    long msg_type;
    double total;
    double average;
} result;

int main() {
    key_t key;
    int msgid;

    // Generate a unique key for message queue
    key = ftok("msgqueue", 65);

    // Access the message queue
    msgid = msgget(key, 0666 | IPC_CREAT);

    // Receive the burst times from the message queue
    msgrcv(msgid, &message, sizeof(message), 1, 0);

    printf("Receiver: Burst times received.\n");

    // Calculate total and average burst time
    int total = 0, count = 0;
    for (int i = 0; i < MAX && message.burst_times[i] != 0; i++) {
        total += message.burst_times[i];
        count++;
    }

    double average = (double)total / count;

    // Prepare result to send back
    result.msg_type = 2; // Different message type
    result.total = total;
    result.average = average;

    // Send result back to the parent
    msgsnd(msgid, &result, sizeof(result), 0);

    printf("Receiver: Total and average burst time sent to parent.\n");

    return 0;
}
