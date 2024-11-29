#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>

#define maxsize 10

int count = 0;

sem_t empty, full;
pthread_mutex_t mutex;
int buffer[maxsize];

int produceItem(){
    static int item = 0;
    return ++item;
}

int consumeItem(int item){
    printf("Consumed item : %d\n", item);
}

void* producer(void* arg){
    int n = (int) arg;
    while(n){
        int item = produceItem();

        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        buffer[count] = item;
        count++;
        printf("Produced item : %d\n", item);

        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        n--;
        sleep((rand()%100)/100);
    }
    return NULL;
}

void* consumer(void* arg){
    int n = (int)arg;
    while(n){
        sem_wait(&full);

        pthread_mutex_lock(&mutex);

        int item = buffer[--count];
        consumeItem(item);

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        n--;
        sleep((rand()%100)/100);
    }
}

int main(){
    int n;
    printf("Enter number of items : ");
    scanf("%d", &n);
    pthread_t prod, cons;

    sem_init(&empty, 0, n);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&prod, NULL, producer, (void*)n);
    pthread_create(&cons, NULL, consumer, (void*)n);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}