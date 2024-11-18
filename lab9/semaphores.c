#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>

sem_t sem;

void threadAfunction(void* arg){
    
}

void threadBfunction(void *arg){

}

int main(){

    pthread_t threadA, threadB;

    pthread_create(&threadA, NULL, threadAfunction, NULL);
    pthread_create(&threadB, NULL, threadBfunction, NULL);

    //Second argument to check if semaphore is between threads or processes - 0 for threads & 1 for processes
    sem_init(&sem, 0, 3);   // Using the same semaphore for printing all the statements.

    pthread_join(&threadA, NULL);
    pthread_join(&threadB, NULL);



    return 0;
}