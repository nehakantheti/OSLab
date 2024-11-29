#include<stdio.h>
#include<pthread.h>

#define num_threads 5
#define num_count 1000
#define CHUNK_SIZE (num_count/num_threads)

long thread_sum[num_threads];

void* partial_sum(void* arg){
    long thread_id = (long)arg;
    long start = thread_id * CHUNK_SIZE + 1;
    long end = (thread_id + 1) * CHUNK_SIZE;
    long local_sum = 0;

    for(int i =start; i <= end;i++){
        local_sum += i;
    }
    thread_sum[thread_id] = local_sum;
    pthread_exit(NULL);
}

int main(){
    pthread_t threads[num_threads];
    long total_sum = 0;

    for(int i = 0; i < num_threads;i++){
        pthread_create(&threads[i], NULL, partial_sum, (void*)i);
    }

    for(int i = 0; i < num_threads;i++){
        pthread_join(threads[i], NULL);
    }

    for(int i = 0; i < num_threads;i++){
        total_sum += thread_sum[i];
    }

    printf(" Sum is %d\n", total_sum);
    return 0;
}