#include <unistd.h>
#include "shared_memory.h"

int open_shared_mem_object(int oflag, mode_t mode)
{
    int fd = shm_open(SHRD_MEM_OBJ, oflag, mode);
    if(fd == SYS_FAILURE)
    {
        HANDLE_ERROR("error at creating shared memory object\n");
    }
    return fd;
}
void extend_memory_object(int fd, int size){
    if(ftruncate(fd,size)==SYS_FAILURE){

        HANDLE_ERROR("error at extending memory object\n");
    }
}

void* map_shared_memory(void *addr,size_t length,int prot,int flags, int fd, off_t offset){
    void *ret = mmap(addr, length, prot, flags, fd, offset);
    if(ret == MAP_FAILED)
    {
        HANDLE_ERROR("error at mapping shared memory\n");
    }
    return ret;
}

void init_semaphore(sem_t *semaphore, int pshared, unsigned int value){
    if(sem_init(semaphore, pshared, value) == SYS_FAILURE)
    {
        HANDLE_ERROR("error with function init_semaphore\n");
    }
}

void wait_semaphore(sem_t *semaphore){
    if(sem_wait(semaphore) == SYS_FAILURE)
    {
        HANDLE_ERROR("error with function wait_semaphore\n");
    }
}

void post_semaphore(sem_t *semaphore){
    if(sem_post(semaphore) == SYS_FAILURE){

        HANDLE_ERROR("error at posting semaphore\n");
    }
}
void destroy_semaphore(sem_t* semaphore){
    if(sem_destroy(semaphore) == SYS_FAILURE){

        HANDLE_ERROR("error at destroying semaphore\n");

    }
}
