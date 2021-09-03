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
        //char* value;
        char result='c';
        //scanf("%s",value);
        close(mastertoslave[READ]);
        close(slavetomaster[WRITE]);
        read(slavetomaster[READ],&result,sizeof(char));
        printf("%c\n",result);
        printf("execution finished\n");
    }
    else{
        close(mastertoslave[WRITE]);
        close(slavetomaster[READ]);
        dup2(mastertoslave[READ],STDIN_FILENO);
        dup2(slavetomaster[WRITE],STDOUT_FILENO);
        close(mastertoslave[READ]);
        close(slavetomaster[WRITE]);
        if(execl("./slave","./slave",(char*) 0)==-1){
            printf("error with execl\n");
            return 2;
        }
    }
    wait(&pid);
    return 0;
}