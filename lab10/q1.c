#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define n 5

sem_t s[5],mutex;
char state[5];
int id[5];

int left(int i)
{
    return (i+n-1)%n;
}

int right(int i)
{
    return (i+1)%n;
}

void think(int i)
{
    printf("Philosopher %d is thinking. \n",i);
    sleep(1);
}

void eat(int i)
{
    printf("Philosopher %d is eating. \n",i);
    sleep(1);
}

void test(int i)
{
    if(state[i]=='h' && state[left(i)]!='e' && state[right(i)]!='e')
    {
        state[i]='e';
        sem_post(&s[i]);
    }
}

void take_fork(int i)
{
    sem_wait(&mutex);
    state[i]='h';
    printf("Philosopher %d is hungry. \n",i);
    test(i);
    sem_post(&mutex);
    sem_wait(&s[i]);
}

void put_fork(int i)
{
    sem_wait(&mutex);
    state[i]='t';
    printf("Philosopher %d completed eating. \n",i);
    test(left(i));
    test(right(i));
    sem_post(&mutex);
}

void *philosopher(void *num)
{
    int i=*(int *)num;
    
    while(1)
    {
        think(i);
        take_fork(i);
        eat(i);
        put_fork(i);
    }
}

int main() {
    
    sem_init(&mutex, 0, 1);
    sem_init(&s[0], 0, 0);
    sem_init(&s[1], 0, 0);
    sem_init(&s[2], 0, 0);
    sem_init(&s[3], 0, 0);
    sem_init(&s[4], 0, 0);
    
    for(int i=0;i<5;i++)
    {
        state[i]='t';
        id[i]=i;
    }
    
    pthread_t r1, r2, r3, r4, r5;
    
    pthread_create(&r1, NULL, philosopher, &id[0]);
    pthread_create(&r2, NULL, philosopher, &id[1]);
    pthread_create(&r3, NULL, philosopher, &id[2]);
    pthread_create(&r4, NULL, philosopher, &id[3]);
    pthread_create(&r5, NULL, philosopher, &id[4]);

    pthread_join(r1, NULL);
    pthread_join(r2, NULL);
    pthread_join(r3, NULL);
    pthread_join(r4, NULL);
    pthread_join(r5, NULL);

    sem_destroy(&mutex);
    sem_destroy(&s[0]);
    sem_destroy(&s[1]);
    sem_destroy(&s[2]);
    sem_destroy(&s[3]);
    sem_destroy(&s[4]);

    return 0;
}