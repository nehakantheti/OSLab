#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<unistd.h>
#include<pthread.h>

sem_t wrt;
pthread_mutex_t mutex;
int readcount = 0;
int sharedData = 0;
int numreads = 5;
int numwrites = 2;

void* readfunc(void* arg){
    for(int i = 0; i< numreads;i++){
        int readerid = *(int*)arg;

        pthread_mutex_lock(&mutex);
        
        readcount++;
        printf("Number of reader : %d\n", readcount);

        if(readcount==1){
            sem_wait(&wrt);
        }

        pthread_mutex_unlock(&mutex);

        printf("Reader %d readfunc : %d\n", readerid, sharedData);

        pthread_mutex_lock(&mutex);

        readcount--;
        if(readcount==0){
            sem_post(&wrt);
        }

        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    return NULL;
}

void* writefunc(void* arg){
    for(int i = 0; i < numwrites;i++){
        sem_wait(&wrt);

        sharedData++;
        printf("Writer wrote : %d\n", sharedData);

        sem_post(&wrt);
        sleep(1);
    }
}

int main(){
    pthread_t reader[5], writer;
    int readIds[] = {1,2,3,4,5};

    sem_init(&wrt, 0, 1);
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&writer, NULL, writefunc, NULL);
    for(int i = 0; i < 5;i++){
        pthread_create(&reader[i], NULL, readfunc, &readIds[i]);
    }

    pthread_join(writer, NULL);
    for(int i = 0;i < 5;i++){
        pthread_join(reader[i], NULL);
    }

    sem_destroy(&wrt);
    pthread_mutex_destroy(&mutex);

    return 0;
}