
#include "shared_memory.h"

int main (int argc, char* argv[]){
    
    
    if (setvbuf(stdout, NULL, _IONBF, 0) != 0) {
        HANDLE_ERROR("Error in Setvbuf");
    }
    if (setvbuf(stdin, NULL, _IONBF, 0) != 0) {
        HANDLE_ERROR("Error in Setvbuf");
    }
    int tasksSize = 0;
    if (argc - 1 == 0) {// Pipe 
        char buffer[100] = {0};
        int readBytes = read(STDIN_FILENO,buffer,100);
        if (readBytes < 0) {
            HANDLE_ERROR("Error at reading from stdin of View proccess");
        }
        tasksSize = strtol(buffer, NULL, 10);
    }
    else if (argc - 1 == 1) {// Parametro 
        tasksSize = strtol(argv[1], NULL, 10);
    }
    else {
        HANDLE_ERROR("Error, wrong parameters in vision");
    }
    if (tasksSize == 0) {
        HANDLE_ERROR("Error, no tasks found to be processed");
    }
    int i = 0;
    shmem_t shmem;
    t_sem sem;

    shmem = joinSharedMem(SHRD_MEM_OBJ, tasksSize * BLOCK_SIZE);
    sem = openSem(SEM_OBJ);
    do
    {
        char* result;
        waitSem(&sem);
        result = readSharedMem(&shmem);
        int cantRead = strlen(result);
        if (cantRead > 0)
            i++;
        printf("%s\n", result);
    } while (i<tasksSize);

    closeSharedMed(&shmem);
    closeSem(&sem);
    return 0;
}