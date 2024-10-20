#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>

int main(){

    int pid1 = fork();
    if(pid1){   // Parent Process
        wait(NULL);
        int c2 = fork();
        if(c2){ //Parent after executing C2
            wait(NULL);
            printf("In Parent Process - Parent\n");
            printf("My process id is %d and parent id is %d\n", getpid(), getppid());
        }else{  // Child 2
            int c5 = fork();
            if(c5){ //Parent after executing Child 5 - child 2
                wait(NULL);
                printf("In child process - C2\n");
                printf("My process id is %d and parent id is %d\n", getpid(), getppid());
            }else{
                printf("In child process - C5\n");
                printf("My process id is %d and parent id is %d\n", getpid(), getppid());
            }
        }
    }else{      // Child process
        int c3 = fork();
        if(c3){  //Parent after executing Child 3
            wait(NULL);
            int c4;
            c4 = fork();
            if(c4){ //Parent process after executing Child 4 - Child 1
                wait(NULL);
                printf("In child process - C1\n");
                printf("My process id is %d and parent id is %d\n", getpid(), getppid());
            }else{  //Child 4
                printf("In child process - C4\n");
                printf("My process id is %d and parent id is %d\n", getpid(), getppid());
            }
        }else{  // Child 3
            printf("In child process - C3\n");
            printf("My process id is %d and parent id is %d\n", getpid(), getppid());
        }
    }

    return 0;
}