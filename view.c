
#include "shared_memory.h"

int main (int argc, char* argv[]){
    
    
    if (setvbuf(stdout, NULL, _IONBF, 0) != 0)
    {
        HANDLE_ERROR("Error in Setvbuf");
    }
    if (setvbuf(stdin, NULL, _IONBF, 0) != 0)
    {
        HANDLE_ERROR("Error in Setvbuf");
    }
    int shm_fdr=open_shared_mem_object(O_RDWR, S_IRUSR | S_IWUSR);
    shmem_t* shm_ptr=(shmem_t*)map_shared_memory(PROT_READ,MAP_SHARED,shm_fdr,0);
    int tasksSize = 0;
    if (argc - 1 == 0) // Pipe
    {
        char buffer[100]={0};
        int readBytes=read(STDIN_FILENO,buffer,100);
        if(readBytes<0){
            HANDLE_ERROR("Error at reading from stdin of View proccess");
        }
        tasksSize = atoi(buffer);
        printf("%d",tasksSize);
    }
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