#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define READ 0
#define WRITE 1

int main(){
    int mastertoslave[2],slavetomaster[2];
    if(pipe(mastertoslave)==-1){
        printf("error at pipe mastertoslave\n");
        return 1;
    }
    if(pipe(slavetomaster)==-1){
        printf("error at pipe slavetomaster\n");
        return 2;
    }
    int pid=fork();
    if(pid!=0){
        printf("please enter value\n");
        int value, result;
        scanf("%d",&value);
        printf("begining to solve equations\n");
        close(mastertoslave[READ]);
        close(slavetomaster[WRITE]);
        write(mastertoslave[WRITE],&value,sizeof(int));
        wait(&pid);
        read(slavetomaster[READ],&result,sizeof(int));
        printf("the result is: %d\n",result);
        printf("execution finished\n");
    }
    else{
        close(mastertoslave[WRITE]);
        close(slavetomaster[READ]);
        int y;
        read(mastertoslave[READ],&y,sizeof(int));
        printf("the value is %d\n",y);
        int result= y*5+9-2;
        sleep(2);
        write(slavetomaster[WRITE],&result,sizeof(int));
    }
    return 0;
}