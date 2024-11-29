#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<pthread.h>
#include<unistd.h>

sem_t sem1, sem2;

void* printNumber(void* arg){
    int num = *(int*) arg;
    while(num < 26){
        sem_wait(&sem1);

        printf("%d  ", num++);

        sem_post(&sem2);
    }

}

void* printLetter(void* arg){
    int num = *(int*) arg;
    while(num < 26){
        sem_wait(&sem2);

        printf("%c  \n", num+65);
        num++;
        sem_post(&sem1);
    }
}

int main(){

    sem_init(&sem1, 0, 1);
    sem_init(&sem2, 0, 0);

    int i = 0;

    pthread_t nums, lets;

    pthread_create(&nums, NULL, printNumber, &i);
    pthread_create(&lets, NULL, printLetter, &i);


    pthread_join(nums, NULL);
    pthread_join(lets, NULL);


    return 0;
}