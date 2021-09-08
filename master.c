#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/select.h>

typedef struct
{
    pid_t pid;
    int receiver;     // Devuelve resultados, slaveToMaster.
    int sender;   // Manda tarea, masterToSlave.
    int ntasks;     // Tareas siendo ejecutadas por el esclavo.
    int flagEOF;
} slave_t;
typedef struct
{
    int totalTasks;
    int taskCompleted;
    char** paths;
}taskCount_t;

#define SLAVE_INIT 5
#define SLAVE_COUNT(c) ((c<SLAVE_INIT)? c : SLAVE_INIT) 
#define READ 0
#define WRITE 1
#define PATH_SLAVE "./Slave"
#define INITIAL_TASKS 1
#define BUFFER_SIZE 4000
#define HANDLE_ERROR(msg)   \
    do                      \
    {                       \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while (0)

int createSlaves(char** paths,int dimSlaves, slave_t slaves[]);
int getRunningSlaves(slave_t slaves[],int slaveCount);
void sendNewTask(int fileDescriptor,taskCount_t taskStructure,int slavesCount);

int main(int argc,char** argv){
    taskCount_t tasks;
    tasks.totalTasks=argc-1;
    tasks.taskCompleted=0; 
    tasks.paths=argv;
    int slaveCount=SLAVE_COUNT(tasks.totalTasks), pendingTasks;
    slave_t slaves[slaveCount];
    createSlaves(tasks.paths,slaveCount,slaves);
    fd_set readSet;
    while(tasks.taskCompleted<tasks.totalTasks){
        char buffer[BUFFER_SIZE]={0};
        FD_ZERO(&readSet);
        int nfds=0, i, j;
        for(i=0;i<slaveCount;i++){
            slave_t slave=slaves[i];
            if (slave.flagEOF==0){
                FD_SET(slaves[i].receiver,&readSet);
                if(nfds<=slaves[i].receiver)
                    nfds=slaves[i].receiver;
            }
        }
        int retval=select(nfds+1,&readSet,NULL,NULL,NULL);
        if(retval==-1){
            HANDLE_ERROR("Error at select function");
        }
        for(j=0;j<slaveCount;j++){
            if(FD_ISSET(slaves[j].receiver,&readSet)){
                int bytesRead=read(slaves[j].receiver,buffer,BUFFER_SIZE);
                pendingTasks=tasks.totalTasks-tasks.taskCompleted-slaveCount;
                tasks.taskCompleted++;
                if(pendingTasks>0){
                    sendNewTask(slaves[j].receiver,tasks,slaveCount);
                }
                else{
                    close(slaves[j].receiver);
                    close(slaves[j].sender);
                    slaves[j].flagEOF=1;
                    slaveCount--;
                }
                if(bytesRead==-1){
                HANDLE_ERROR("error at reading from slave");
                }
                buffer[bytesRead]=0;
                printf("%s\n",buffer); 
            }
        }
    }
    return 0;
}

int createSlaves(char** paths,int dimSlaves, slave_t slaves[])
{
    int i;
    for (i = 0; i < dimSlaves; i++)
    {
        int masterToSlave[2],slaveToMaster[2];
        pipe(masterToSlave);
        if (pipe(masterToSlave) < 0)
        {
            HANDLE_ERROR("Pipe Error Master");
        }
        slaves[i].sender = masterToSlave[WRITE];

        pipe(slaveToMaster);
        if (pipe(slaveToMaster) < 0)
        {
            HANDLE_ERROR("Pipe Error Slave ");
        }
        slaves[i].receiver = slaveToMaster[READ];
        slaves[i].flagEOF = 0;
        int pid;
        if ((pid = fork()) == -1)
        {
            HANDLE_ERROR("Fork Error");
        }
        else if (pid == 0)
        {
            if (close(masterToSlave[WRITE]) < 0)
            {
                HANDLE_ERROR("Error closing slave WRITE to master");
            }
            if (dup2(masterToSlave[READ],STDIN_FILENO) < 0)//REDIRECCION STDIN DEL SLAVE AL PIPE
            {
                HANDLE_ERROR("Error dupping pipe");
            }

            if (close(slaveToMaster[READ]) < 0)
            {
                HANDLE_ERROR("Error closing slave WRITE to master");
            }
            if (dup2(slaveToMaster[WRITE],STDOUT_FILENO) < 0) //REDIRECCION STDOUT DEL SLAVE AL PIPE
            {
                HANDLE_ERROR("Error dupping pipe");
            }
            char* arguments[3];
            arguments[0]=PATH_SLAVE;
            arguments[1]=paths[i+1];
            arguments[2]=NULL;
            if (execv(PATH_SLAVE, arguments) < 0)
            {
                HANDLE_ERROR("Execv Error");
            }
        }
        else if (pid < 0)
        {
            HANDLE_ERROR("Fork Error");
        }
        if (close(masterToSlave[READ]) < 0)
        {
            HANDLE_ERROR("Error closing master READ to slave");
        }
        if (close(slaveToMaster[WRITE]) < 0)
        {
            HANDLE_ERROR("Error closing slave WRITE to master");
        }
        if(pid>0)
            slaves[i].pid = pid;
    }
    return 0;
}
void sendNewTask(int fileDescriptor,taskCount_t taskStructure,int slavesCount){
    int taskIndex=taskStructure.taskCompleted+slavesCount-1;
    char* newTask=taskStructure.paths[taskIndex];
    write(fileDescriptor,newTask,strlen(newTask));
}