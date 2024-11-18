// CS22B1081
// Neha Kantheti
#include<stdio.h>
#include<pthread.h>
#include<sys/wait.h>
#include<unistd.h>

int globalvar = 0;

void* func(void* args){
    int localvar = (pthread_t)args;
    printf("\n----------Thread %d------------\n",localvar);
    globalvar++;
    printf("Thread Id : %lu\n", pthread_self());
    printf("Process Id : %d\n", getpid());
    printf("Gobal variable is increased by 1 : %d\n", globalvar);
    printf("Address of global variable : %p\n", &globalvar);
    printf("Local variable before manipulating : %d\n", localvar);
    printf("Address of local variable : %p\n", &localvar);  
    localvar++;      
    printf("Local variable after manipulating(increased by 1) : %d\n", localvar);
    printf("Address of local variable : %p\n", &localvar);  
    // pthread_exit(NULL);      

}

void threads(){
    pthread_t t1,t2;
    pthread_create(&t1, NULL, func, (void*)(int)1);
    pthread_join(t1, NULL);
    pthread_create(&t2, NULL, func, (void*)(int)2);
    pthread_join(t2, NULL);
}

int main(){
    pid_t pid;
    pid = fork();
    if(pid < 0){
        //Error
        printf("Error in forking");
    }else if(pid == 0){
        // Child Process
        printf("In Child Process\n");
        threads();
        printf("\tChild process finished\n");
    }else{
        //Parent Process
        printf("In Parent Process\n");
        printf("Waiting for child process to finish!\n");
        wait(NULL);
        threads();
        printf("\tParent Process finished\n");
    }
    // pthread_exit(NULL);
    return 0;
}