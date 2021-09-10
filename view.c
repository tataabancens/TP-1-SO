
#include "shared_memory.h"

int main (int argc, char* argv[]){

    /*
    if (setvbuf(stdout, NULL, _IONBF, 0) != 0)
    {
        HANDLE_ERROR("Error in Setvbuf");
    }
    if (setvbuf(stdin, NULL, _IONBF, 0) != 0)
    {
        HANDLE_ERROR("Error in Setvbuf");
    }
   
    int tasksSize = 0;
    if (argc - 1 == 0) // Pipe
    {
        char tasksRead[BLOCK_SIZE]={0};
        int bytesRead;
        if ((bytesRead = read(STDIN_FILENO, tasksRead, 20)) == -1)
            HANDLE_ERROR("Error in read from pipe in vision");
        tasksRead[bytesRead] = 0;
        tasksSize = strtol(tasksRead, NULL, 10);
    }*/
    /*else if (argc - 1 == 1) // Parametro
    {
        tasksSize = strtol(argv[1], NULL, 10);
    }
    else
    {
        HANDLE_ERROR("Error, wrong parameters in vision");
    }
    
    if (tasksSize == 0)
    {
        HANDLE_ERROR("Error, no tasks found to be processed");
    }*/
   /* int shm_fdr=open_shared_mem_object(O_RDONLY,NULL);
    void* shm_ptr=map_shared_memory(NULL,BLOCK_SIZE,PROT_READ,MAP_SHARED);
    sem_t sem;
    init_semaphore(&sem,1,0);
    int i = 0;
    do
    {
        char result[BLOCK_SIZE] = {0};
        wait_semaphore(&sem);
        memcpy(result,(char*) shm_ptr,BLOCK_SIZE);
        int cantRead = strlen(result);
        if (cantRead > 0)
            i++;
        printf("%s\n", result);
    } while (1);
    close(shm_fdr);*/
    return 0;
}