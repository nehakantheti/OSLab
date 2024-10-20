#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>

int main(){

    pid_t pid1,pid2;

    if(fork()){ //Parent Process
        int status1;
        wait(&status1);
        if(!fork()){    //Child2
            printf("In child 2 - value is 1\n");
        }else{
            int status2;
            wait(&status2);
            printf("In Parent Process - Value is 2\n");
        }

    }else{
        printf("In child 1 - Value is 3\n");
    }

    return 0;
}