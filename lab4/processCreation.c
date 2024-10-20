#include<stdio.h>
#include<ctype.h>
#include<unistd.h>
#include<limits.h>
#define MAX_SIZE 7


int input[MAX_SIZE] = {0};

int main(){
    printf("Give %d inputs : ", MAX_SIZE);
    for(int i = 0; i < MAX_SIZE;i++){
        scanf("%d", &input[i]);
    }

    pid_t p1 = fork();

    printf("Initial p1 = %d\n", p1);

    if(p1 < 0){     // Error in creating in process
        printf("Error!\n");
    }else if(p1 > 0){
        for(int i = 0;i< MAX_SIZE;i++){
            input[i]-=5;
        }

        int minn = INT_MAX;
        for(int i = 0; i < MAX_SIZE; i++){
            printf("array[%d] = %d and address = %p\n", i, input[i], &input[i]);
            if(minn > input[i])
                minn = input[i];
        }
        printf("Minimum Element : %d\n", minn);

        printf("Process id : %d and Parent Id : %d\n", getpid(), getppid());
    }else{
        for(int i = 0;i< MAX_SIZE;i++){
            input[i]+=2;
        }

        int maxx = INT_MIN;
        for(int i = 0; i < MAX_SIZE; i++){
            printf("array[%d] = %d and address = %p\n", i, input[i], &input[i]);
            if(maxx < input[i])
                maxx = input[i];
        }
        printf("Maximum Element : %d\n", maxx);

        printf("Process id : %d and Parent Id : %d\n", getpid(), getppid());
    }


    return 0;
}