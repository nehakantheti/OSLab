// K Bala Sai Manvitha - CS22B1030

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>

#define N 5

sem_t mutex, S[N];
char *state[5] = {"thinking","thinking", "thinking", "thinking", "thinking"};

void test(int pId)
{
    if(strcmp(state[pId],"hungry") == 0 && strcmp(state[(pId+N-1) % N], "eating") != 0 && strcmp(state[(pId+1)%N], "eating") != 0 ){
        state[pId] = "eating";
        sem_post(&S[pId]);
    }
}

void takeFork(int pId)
{
    sem_wait(&mutex);
    state[pId] = "hungry";
    printf("Philosopher %d is hungry!!\n", pId);

    test(pId);

    sem_post(&mutex);
    sem_wait(&S[pId]);
}

void putFork(int pId)
{
    sem_wait(&mutex);
    state[pId] = "thinking";

    test((N+pId-1)%N);
    test((pId+1)%N);

    sem_post(&mutex);

}

void *philosopher(void *arg)
{
    int pId = *(int*)arg;
    for(int i=0; i<2; i++){
        printf("Philosopher %d is thinking!\n", pId);
        sleep(1);
        takeFork(pId);
        printf("Philosopher %d is eating!\n", pId);
        sleep(1);
        putFork(pId);
        sleep(1);
    }
}

int main()
{
    pthread_t P[N];
    sem_init(&mutex, 0, 1);

    for(int i=0;i<N;i++)
    {
        sem_init(&S[i], 0, 0);
    }
    int philosopher_ids[N]; 
    for(int i=0; i<N; i++){
        philosopher_ids[i] = i; 
        pthread_create(&P[i], NULL, philosopher, &philosopher_ids[i]);
    }

    for(int i=0;i<5;i++)
    {
        pthread_join(P[i], NULL);
    }

    sem_destroy(&mutex);;
    return 0;
}