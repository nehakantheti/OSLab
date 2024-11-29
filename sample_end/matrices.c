#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<stdint.h>

int M1[3][3], M2[3][3], M3[3][3];
int A[3][3] = {{1,1,1},
                {2,2,2},
                {3,3,3}};
int B[3][3] = {{3,3,3},
                {2,2,2},
                {1,1,1}};

void* thread_func(void* arg){
    int threadid = (intptr_t)arg;
    if(threadid == 0){
        for(int i = 0 ; i < 3;i++){
            for(int j = 0 ; j < 3;j++){
                M1[i][j] = A[i][j] + B[i][j];
            }
        }
    }else if(threadid == 1){
        for(int i = 0 ; i < 3;i++){
            for(int j = 0 ; j < 3;j++){
                M2[i][j] = A[i][j] - B[i][j];
            }
        }
    }else{
        for(int i = 0 ; i < 3;i++){
            for(int j = 0 ; j < 3;j++){
                for(int k =0; k <3;k++){
                    M3[i][j] = A[i][k] * B[k][j];
                }
            }
        }
    }

}

void print(int arr[][3]){
    for(int i = 0; i < 3;i++){
        for(int j = 0; j < 3;j++){
            printf("%d ", arr[i][j]);
        }printf("\n");
    }
}

int main(){
    pthread_t t1,t2,t3;
    pthread_create(&t1, NULL, thread_func, (void*)0);
    pthread_create(&t2, NULL, thread_func, (void*)1);
    pthread_create(&t3, NULL, thread_func, (void*)2);
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    printf("Matrix 1: \n");
    print(M1);

    printf("Matrix 2 : \n");
    print(M2);

    printf("Matrix 3 : \n");
    print(M3);

    return 0;
}