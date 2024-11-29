#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>

sem_t semA2, semB2;

void* threadAfunction(void* arg){
    printf("Statement A1\n");

    sem_post(&semB2);

    sem_wait(&semA2);

    printf("Statement A2\n");
}

void* threadBfunction(void *arg){
    printf("Statement B1\n");

    sem_post(&semA2);

    sem_wait(&semB2);

    printf("Statement B2\n");
}

int main(){

    pthread_t threadA, threadB;

    sem_init(&semA2, 0, 0);
    sem_init(&semB2, 0, 0);

    pthread_create(&threadA, NULL, threadAfunction, NULL);
    pthread_create(&threadB, NULL, threadBfunction, NULL);

    pthread_join(threadA, NULL);
    pthread_join(threadB, NULL);

    sem_destroy(&semA2);
    sem_destroy(&semB2);

    return 0;
}