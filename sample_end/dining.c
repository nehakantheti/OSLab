#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>

#define n 5

sem_t s[5], mutex;
char state[5];
int id[5];

void think(int i){
    printf("Philosopher %d is thinking...\n", i);
    sleep(0.5);
}

void eat(int i){
    printf("Philosopher %d is eating...\n", i);
    sleep(0.5);
}

void take_fork(int i){
    sem_wait(&mutex);
    state[i] = 'h';
    printf("Philosopher %d is hungry\n", i);
    test(i);    // Eats if both forks are free.
    sem_post(&mutex);
    sem_wait(&s[i]);
}

void put_fork(int i){
    sem_wait(&mutex);
    state[i] = 't';
    printf("Philosopher %d finished eating\n", i);
    test((i+5-1)%5);    //left
    test((i+1)%5);      //right
    sem_post(&mutex);
}

void* philosopher(void* arg){
    int i = *(int*)arg;

    while(1){
        think(i);
        take_fork(i);
        eat(i);
        put_fork(i);
    }
}

int main(){
    sem_init(&mutex, 0, 1);
    for(int i = 0; i < 5;i++){
        sem_init(&s[i], 0, 0);
    }

    for(int i = 0; i < 5;i++){
        state[i] = 't';
        id[i] = i;
    }

    pthread_t philosophers[5];

    for(int i = 0; i < 5;i++){
        pthread_create(&philosophers[i], NULL, philosopher, &id[i]);
    }

    for(int i = 0; i < 5;i++){
        pthread_join(philosophers[i], NULL);
    }

    sem_destroy(&mutex);
    for(int i = 0; i < 5;i++){
        sem_destroy(&s[i]);
    }

    return 0;
}