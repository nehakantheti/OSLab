#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<stdint.h>
#include<sys/wait.h>

int glob1 = 100;

void* thread(void* arg){
    int var = (intptr_t)arg;
    printf("var1 : %d\n", var);
    var++;
    printf("var1 : %d\n", var);
    glob1+=5;
    printf("glob1 : %d\n", glob1);
    pthread_exit(NULL);
}

int main(){
    pid_t p1, p2;
    pthread_t t1, t2;
    
    p1 = fork();
    if(p1 == 0){    // Child process
    int var = 0;
        pthread_create(&t1, NULL, thread, (void*)var);
        pthread_join(t1, NULL);
        pthread_create(&t2, NULL, thread, (void*)var);

        pthread_join(t2, NULL);
    }else{
        int var = 0;
        wait(NULL);
        pthread_create(&t1, NULL, thread, (void*)var);
        pthread_join(t1, NULL);
        pthread_create(&t2, NULL, thread, (void*)var);

        pthread_join(t2, NULL);
    }
    return 0;
}