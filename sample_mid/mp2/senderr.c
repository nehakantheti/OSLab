#include<stdio.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/msg.h>

#define MAX 10

struct msg_buffer{
    long msg_type;
    int burst_times[MAX];
}message;

int main(){
    key_t key;
    int msg_id;

    key = ftok("msgqueue", 65); // Using some unique token to create

    msg_id = msgget(key, IPC_CREAT | 0666);

    message.msg_type = 1;

    int n;
    printf("Enter numberof btimes : ");
    scanf("%d", &n);

    printf("Enter burst times : ");
    for(int i = 0; i < n;i++){
        scanf("%d", &message.burst_times[i]);
    }

    msgsnd(msg_id, &message,sizeof(message), 0);

    printf("Sender : Burst times sent!\nSender program terminating\n");

    return 0;
}