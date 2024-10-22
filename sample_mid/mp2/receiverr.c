#include<stdio.h>
#include<stdlib.h>
#include<sys/msg.h>
#include<sys/ipc.h>
#include<unistd.h>


#define MAX 10

struct msg_buffer{
    long msg_type;
    int burst_times[MAX];
}message;

struct result_buffer{
    long msg_type;
    int total;
    int average;
}result;

int main(){
    printf("In reciver program \n");
    int msg_id;
    key_t key;

    key = ftok("msgqueue", 65);
    msg_id = msgget(key, IPC_CREAT | 0666);

    msgrcv(msg_id, &message, sizeof(message), 1, 0);
    for(int i = 0; i < 3;i++)
        printf("%d\n", message.burst_times[i]);

    printf("Receiver : Burst times received!\n");

    int count = 0;
    // result.total = 0;
    // result.average = 0;
    int total;
    for(int i = 0; i < MAX && message.burst_times[i] != 0;i++){
        total += message.burst_times[i];
        count++;
    }
    double average = (double)total / (double)count;
    printf("COUNT + %d\n", count);
    
    result.msg_type = 2;
    printf("TOTAL = %d, AVERAGE = %f\n", total, average);
    result.total = total;
    result.average = average;
    // result.average = (double)result.total/count;

    msgsnd(msg_id, &result, sizeof(result), 0);

    printf("Receiver : Total and Average sent to parent process\n");

    return 0;
}