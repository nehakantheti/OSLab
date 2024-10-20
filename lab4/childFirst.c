#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>

int compare(const void* a, const void* b)
{
    return (*(int*)a - *(int*)b);
}

int main(){

    pid_t process1, childPID1, childPID2;
    process1 = fork();
    int status;

    if(process1 < 0){       // Error!
        printf("Error!");
    }else if(process1 > 0){ // Parent process
        childPID1 = wait(&status);
        printf("In parent process!\n");
        int n;
        printf("Enter number of elements : ");
        scanf("%d", &n);
        int *arr = (int*)malloc(n*sizeof(int));
        printf("Enter %d elements : ", n);
        for(int i = 0 ; i< n ; i++){
            scanf("%d", &arr[i]);
        }
        
        printf("Elements : \n");
        for(int i = 0 ; i < n ; i++){
            printf("%d with address %p\n", arr[i], &arr[i]);
        }

        
        qsort(arr, n, sizeof(int), compare);
        
        printf("Sorted Array : ");
        for(int i = 0; i< n;i++){
            printf("%d ", arr[i]);
        }printf("\n");

        printf("In Parent process - Process id = %d, Parent id = %d\n", getpid(), getppid());
        printf("Terminated child process - Child Process id = %d, Status = %d\n", childPID1, status);

    }else{            //Child Process
        printf("In child process !\n");
        int n, status;
        printf("Enter number of elements : ");
        scanf("%d", &n);
        int *childArr = (int*)malloc(n*sizeof(int));
        printf("Enter %d elements : ", n);
        for(int i = 0 ; i< n ; i++){
            scanf("%d", &childArr[i]);
        }

        printf("Elements : \n");
        for(int i = 0 ; i < n ; i++){
            printf("%d with address %p\n", childArr[i], &childArr[i]);
        }

        printf("Reversed array : ");
        for(int i = n-1;i>=0;i--){
            printf("%d with address %p\n", childArr[i], &childArr[i]);
        }

    }

    return 0;
}