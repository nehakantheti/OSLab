#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <ctype.h>

// Structure for message queue
struct msg_buffer {
    long msg_type;
    char msg_text[100];
} message;

    // Create message queue and return id
    // msgid = msgget(key, 0666 | IPC_CREAT);

    // Case 1: Non-blocking message reception using IPC_NOWAIT (msg_type = 1)
    /*
    if (msgrcv(msgid, &message, sizeof(message), 1, IPC_NOWAIT) == -1) {
        if (errno == ENOMSG) {
            printf("No message available for msg_type = 1.\n");
        } else {
            perror("Error receiving message");
        }
    } else {
        printf("Received message from Sender1: %s", message.msg_text);
        printf("Message length: %ld\n", strlen(message.msg_text) - 1); // excluding newline
        printf("Number of vowels: %d\n", count_vowels(message.msg_text));
    }
    */

    // Case 2: Blocking message reception (msg_type = 2, flag = 0)
    /*
    msgrcv(msgid, &message, sizeof(message), 2, 0);
    printf("Received message from Sender2: %s", message.msg_text);
    printf("Message length: %ld\n", strlen(message.msg_text) - 1); // excluding newline
    printf("Number of vowels: %d\n", count_vowels(message.msg_text));
    */

    // Case 3: Truncated message reception using MSG_NOERROR (msg_type = 1)
    /*
    msgrcv(msgid, &message, 20, 1, MSG_NOERROR);  // Truncated to fit 20 bytes
    printf("Truncated message received from Sender1: %s\n", message.msg_text);
    printf("Message length: %ld\n", strlen(message.msg_text));
    printf("Number of vowels: %d\n", count_vowels(message.msg_text));
    */

    // Case 4: Receiving messages in reverse order of priority (msg_type = -1)
    /*
    // msg_type = -1 means the lowest priority message will be received.
    msgrcv(msgid, &message, sizeof(message), -1, 0);
    printf("Received message with the lowest priority: %s\n", message.msg_text);
    printf("Message length: %ld\n", strlen(message.msg_text));
    printf("Number of vowels: %d\n", count_vowels(message.msg_text));
    */

    // Case 5: Receiving a specific message type (msg_type = 2)
    /*
    // In this case, we explicitly specify the message type = 2.
    msgrcv(msgid, &message, sizeof(message), 2, 0);
    printf("Received specific message type 2: %s\n", message.msg_text);
    printf("Message length: %ld\n", strlen(message.msg_text));
    printf("Number of vowels: %d\n", count_vowels(message.msg_text));
    */

    // Destroy message queue
    // msgctl(msgid, IPC_RMID, NULL);


// Function to count vowels in a message
int count_vowels(char *msg) {
    int vowels = 0;
    for (int i = 0; i < strlen(msg); i++) {
        char ch = tolower(msg[i]);
        if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u') {
            vowels++;
        }
    }
    return vowels;
}

int main() {
    key_t key;
    int msgid;

    // Generate unique key
    // key = ftok("progfile", 65);
    key = 1081;

    // Create message queue and return id
    msgid = msgget(key, 0666 | IPC_CREAT);

    // Receive message from Sender1 (msg_type = 1)
    msgrcv(msgid, &message, sizeof(message), 1, 0);
    printf("Receiver: Received message from Sender1: %s", message.msg_text);
    printf("Message length: %ld\n", strlen(message.msg_text) - 1); // excluding newline
    printf("Number of vowels: %d\n", count_vowels(message.msg_text));

    // Receive message from Sender2 (msg_type = 2)
    msgrcv(msgid, &message, sizeof(message), 2, 0);
    printf("Receiver: Received message from Sender2: %s", message.msg_text);
    printf("Message length: %ld\n", strlen(message.msg_text) - 1); // excluding newline
    printf("Number of vowels: %d\n", count_vowels(message.msg_text));

    // To destroy the message queue
    msgctl(msgid, IPC_RMID, NULL);

    return 0;
}
