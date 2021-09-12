
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
    // int shm_fdr=open_shared_mem_object(O_RDWR, S_IRUSR | S_IWUSR);
    // shmem_t* shm_ptr=(shmem_t*)map_shared_memory(PROT_READ,MAP_SHARED,shm_fdr,0);
    int tasksSize = 0;
    if (argc - 1 == 0) // Pipe
    {
        char buffer[100]={0};
        int readBytes=read(STDIN_FILENO,buffer,100);
        if(readBytes<0){
            HANDLE_ERROR("Error at reading from stdin of View proccess");
        }
        tasksSize = atoi(buffer);
    }
    else if (argc - 1 == 1) // Parametro
    {
        tasksSize = atoi(argv[1]);
    }
    else
    {
        HANDLE_ERROR("Error, wrong parameters in vision");
    }
    if (tasksSize == 0)
    {
        HANDLE_ERROR("Error, no tasks found to be processed");
    }
    int i = 0;
    do
    {
        char result[BLOCK_SIZE]={0};
        //wait_semaphore(&shm_ptr->sem1);
        // memcpy(result,shm_ptr->buf,shm_ptr->cnt);
        int cantRead = strlen(result);
        if (cantRead > 0)
            i++;
        printf("%s\n", result);
        //post_semaphore(&shm_ptr->sem1);
    } while (i<tasksSize);
    return 0;
}