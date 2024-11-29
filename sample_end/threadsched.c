#include<stdio.h>
#include<stdlib.h>
#include<sched.h>
#include<unistd.h>
#include<pthread.h>

#define numthreads 5

void* thread_function(void* arg){
    int thread_id = *(int*) arg;
    printf("Thread %d : started execution\n",thread_id);

    sleep(0.5);

    printf("Thread %d : Finished execution\n",thread_id);
    pthread_exit(NULL);
}

void set_thread_priority(pthread_attr_t* attr, int priority, int policy){
    struct sched_param param;
    param.sched_priority = priority;
    pthread_attr_setschedpolicy(attr,policy);
    pthread_attr_setschedparam(attr, &param);
}

int main(){
    pthread_t threads[numthreads];
    pthread_attr_t attr;

    int threadids[numthreads];
    int policy;

    pthread_attr_init(&attr);
    policy = SCHED_FIFO;
    printf("Policy sset to FIFO\n");

    for(int i = 0; i < numthreads;i++){
        threadids[i] = i;
        int priority = 10 + i;
        // set_thread_priority(&attr, priority, policy);
        struct sched_param param;
        param.sched_priority = priority;
        pthread_attr_setschedpolicy(&attr,policy);
        pthread_attr_setschedparam(&attr, &param);
        pthread_create(&threads[i], &attr, thread_function, (void*)&threadids[i]);
    }
    for(int i = 0; i < numthreads;i++){
        pthread_join(threads[i], NULL);
    }

    pthread_attr_destroy(&attr);
    return 0;
}