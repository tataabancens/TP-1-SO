
#include <string.h>
#include <sys/wait.h>
#include <sys/select.h>
#include "shared_memory.h"

typedef struct
{
    pid_t pid;
    int sender;         // Devuelve resultados, slaveToMaster.
    int receiver;       // Manda tarea, masterToSlave.
    int ntasks;         // Tareas siendo ejecutadas por el esclavo.
    int flagEOF;
} slave_t;
#define SLAVE_INIT 2
#define RESULT_PATH "result.txt"
#define SLAVE_COUNT(c) ((c<SLAVE_INIT)? c : SLAVE_INIT) 
#define READ 0
#define WRITE 1
#define WRITE_ONLY 00200
#define PATH_SLAVE "./Slave"
#define INITIAL_TASKS 1
#define BUFFER_SIZE 4096



int createSlaves(char** paths,int dimSlaves, slave_t slaves[], int *taskIndex);
int endSlaves(slave_t slaves[], int slaveCount);
void writeResults(sem_t* semaphore,char* buffer,FILE* file, char* shm_pointer);

void sendNewTask(slave_t slave, char *path, int *taskIndex);

int main(int argc, char** argv){

    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stdin, 0, _IONBF, 0);

    int totalTasks = argc - 1, completedTasks = 0, taskIndex = 1;
    char** paths = argv;
    int slaveCount = SLAVE_COUNT(totalTasks);
    slave_t slaves[slaveCount];
    FILE* fresult=fopen(RESULT_PATH,"w");
    sem_t sem;
    init_semaphore(&sem,1,0);
    int shm_fd=open_shared_mem_object(O_RDWR | O_CREAT,WRITE_ONLY);
    extend_memory_object(shm_fd,BLOCK_SIZE);
    void* shm_ptr=map_shared_memory(NULL,BLOCK_SIZE,PROT_WRITE,MAP_SHARED,shm_fd,0);

    createSlaves(paths,slaveCount,slaves, &taskIndex);

    fd_set readSet;
    
    while (completedTasks < totalTasks) {
        char buffer[BUFFER_SIZE]={0};
        
        FD_ZERO(&readSet);
        int nfds=0, i, j;

        for (i = 0; i < slaveCount; i++) {

            slave_t slave=slaves[i];
            if (slave.flagEOF == 0) {
                FD_SET(slaves[i].sender, &readSet);
                if (nfds <= slaves[i].sender)
                    nfds = slaves[i].sender;
            }
        }
        int retval = select(nfds + 1, &readSet, NULL, NULL, NULL);
        if (retval == -1) {
            HANDLE_ERROR("Error at select function");
        }
        for (j = 0; retval > 0 && j < slaveCount; j++) {
            if (FD_ISSET(slaves[j].sender, &readSet)) {
                
                int bytesRead = read(slaves[j].sender, buffer, BUFFER_SIZE);
                if (bytesRead == -1) {
                    HANDLE_ERROR("error at reading from slave");
                }
                if (bytesRead == 0) {
                    slaves[j].flagEOF = 1;
                } else {
                    buffer[bytesRead]='\n';
                    writeResults(&sem,buffer,fresult,(char*) shm_ptr);
                    completedTasks++;

                    if (taskIndex < totalTasks + 1) {
                        sendNewTask(slaves[j], paths[taskIndex], &taskIndex);
                    }
                }
            }
        }
    }
    destroy_semaphore(&sem);
    endSlaves(slaves, slaveCount);
    close(shm_fd);
    return 0;
}

int createSlaves(char** paths,int dimSlaves, slave_t slaves[], int *taskIndex) {
    int i;
    char* slaveArguments[3];
    slaveArguments[0]=PATH_SLAVE;
    slaveArguments[2]=NULL;

    for (i = 0; i < dimSlaves; i++)
    {
        int masterToSlave[2], slaveToMaster[2];
        pipe(masterToSlave);
        if (pipe(masterToSlave) < 0) {
            HANDLE_ERROR("Pipe Error Master");
        }
        slaves[i].receiver = masterToSlave[WRITE];

        pipe(slaveToMaster);
        if (pipe(slaveToMaster) < 0) {
            HANDLE_ERROR("Pipe Error Slave ");
        }
        slaves[i].sender = slaveToMaster[READ];
        slaves[i].flagEOF = 0;
        int pid;
        if ((pid = fork()) == -1) {
            HANDLE_ERROR("Fork Error");
        }
        else if (pid == 0) {
            if (close(masterToSlave[WRITE]) < 0) {
                HANDLE_ERROR("Error closing slave WRITE to master");
            }
            if (dup2(masterToSlave[READ],STDIN_FILENO) < 0) { //REDIRECCION STDIN DEL SLAVE AL PIPE
                HANDLE_ERROR("Error dupping pipe");
            }

            if (close(slaveToMaster[READ]) < 0) {
                HANDLE_ERROR("Error closing slave WRITE to master");
            }
            if (dup2(slaveToMaster[WRITE],STDOUT_FILENO) < 0) { //REDIRECCION STDOUT DEL SLAVE AL PIPE
                HANDLE_ERROR("Error dupping pipe");
            }

            slaveArguments[1]=paths[i+1];
            
            if (execv(PATH_SLAVE, slaveArguments) < 0) {
                HANDLE_ERROR("Execv Error");
            }
        }
        else if (pid < 0) {
            HANDLE_ERROR("Fork Error");
        }
        if (close(masterToSlave[READ]) < 0) {
            HANDLE_ERROR("Error closing master READ to slave");
        }
        if (close(slaveToMaster[WRITE]) < 0) {
            HANDLE_ERROR("Error closing slave WRITE to master");
        }
        slaves[i].pid = pid;
        (*taskIndex)++;
    }

    return 0;
}
void writeResults(sem_t* semaphore,char* buffer,FILE* file, char* shm_pointer){
    int length=strlen(buffer);
    //memcpy(shm_pointer,buffer,length);
    //post_semaphore(semaphore);
    fwrite(buffer,sizeof(char),length,file);
}
int endSlaves(slave_t slaves[], int slaveCount) {
    int i;
    for (i = 0; i < slaveCount; i++) {
        if (close(slaves[i].receiver) == -1) {
            HANDLE_ERROR("Error closing reciever from slave");
        }

        if (close(slaves[i].sender) == -1) {
            HANDLE_ERROR("Error closing sender from slave");
        }
    }
    for (i = 0; i < slaveCount; i++) {
            if (wait(NULL) == -1) {
                HANDLE_ERROR("Error waiting for slave");
            }
    }
    return 0;
}
        


void sendNewTask(slave_t slave, char *path, int *taskIndex) {
    int dim = strlen(path);

    if ((write(slave.receiver, path, dim)) == -1) {
        HANDLE_ERROR("Error writing to slave");
    }
    (*taskIndex)++;
}
