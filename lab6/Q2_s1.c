#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

// Structure for message queue
struct msg_buffer {
    long msg_type;
    char msg_text[100];
} message;

int main() {
    key_t key;
    int msgid;

    // Generate unique key
    // key = ftok("progfile", 65);
    key = 1081;

    // Create message queue and return id
    msgid = msgget(key, 0666 | IPC_CREAT);
    
    // Define the message type for Sender1
    message.msg_type = 1;

    printf("Sender1: Enter the message: ");
    fgets(message.msg_text, sizeof(message.msg_text), stdin);

    // Send message
    msgsnd(msgid, &message, sizeof(message), 0);

    printf("Message sent by Sender1: %s\n", message.msg_text);

    // msgctl(msgid, IPC_RMID, NULL);


    return 0;
}
