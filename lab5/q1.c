#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<unistd.h>
#include<sys/shm.h>
#include<sys/wait.h>
#define SHM_SIZE 10*sizeof(int)

int main(){
    pid_t pid, childpid;
    int status;
    pid = fork();
    int shmid;
    key_t key = 1505;
    int* s;
    int* shm;  //to read shm's address
    if(pid < 0){    //Error
        printf("Error!");
    }
    else if(pid == 0){    //Child process
        //Creating Shared memory in Parent process and then waiting for child process to start.
        if((shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) < 0){
            perror("shmget");
            exit(1);
        }
        // if((shmid = shmget(key, SHM_SIZE, SHM_RDONLY | 0666)) < 0){
        //     perror("shmget");
        //     exit(1);
        // }
        printf("---------------Shared Memory id : %d------------------\n", shmid);
        shm = shmat(shmid, NULL, 0);    //Attaching SHM in child process.
        if(shm == (int*)-1){
            perror("shmattttt");
            exit(1);
        }
        printf("-----SHM Address in child %p-----\n", shm);
        // char* s;
        s = shm;
        printf("Enter 10 number : \n");
        int num;
        for(int i = 0;i< 10;i++){
            scanf("%d", &num);
            s[i] = num;
        }
        if(!shmdt(shm)){
            printf("Detached succesfully in child process !\n");
        }else{
            printf("Detachment unsuccessful\n");
        }
        printf("Finished child process\n");
        // shmctl(shmid, IPC_RMID, NULL);
    }else{          //Parent Process
        //Creating Shared memory in Parent process and then waiting for child process to start.
        if((shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) < 0){
            perror("shmget");
            exit(1);
        }
        printf("---------------Shared Memory id : %d------------------\n", shmid);
        // If you waitis commented out, the parent process will not wait for the child to finish.
        // This may lead to a race condition where the parent process tries to access the shared memory before the child has finished writing data.
        // As a result, the parent may read incomplete or incorrect data from the shared memory.
        //Parent process also removes shared memory, which then leads to segmentation fault for accessing non existent memory.
        childpid = wait(&status);
        int* shmparent;
        //If not attached, parent process would not be able to access the shared memory.
        shmparent = shmat(shmid, NULL, 0);
        if(shmparent == (int*)-1){
            perror("shmat in parent\n");
            exit(1);
        }
        printf("-----SHM address in parent is %p-----\n", shmparent);
        s = shmparent;
        printf("Enter 10 new numbers : ");
        for(int i = 0; i < 10; i++){
            int n;
            scanf("%d", &n);
            s[i] = n;
            printf("%d th value modified by parent process!\n", i+1);
        }
        printf("Values after modifying : \n");
        for(s = shmparent; s-shmparent < 10;s++){
            printf("%d ", *s);
        }
        printf("Parent process finished successfully, Detaching parent process\n");
        //Detaching from shared memory is important for clean resource management, but it doesn't block subsequent operations if not done.
        //Important to detach shared memory.
        if(!shmdt(shmparent)){
            printf("Detached succesfully in parent process !\n");
        }else{
            printf("Detachment unsuccessful\n");
        }
        // If shmctl is not called, the shared memory segment will not be removed after the parent process completes its work.
        int closed = shmctl(shmid, IPC_RMID, NULL);
        if(!closed){
            printf("Shared memory Removed Successfully\n");
        }else{
            printf("Unsuccessful removal !\n");
        }
    }
    return 0;
}