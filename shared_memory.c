
#include "shared_memory.h"


int open_shared_mem_object(int oflag, mode_t mode)
{
    int fd = shm_open(SHRD_MEM_OBJ, oflag, mode);
    if(fd == SYS_FAILURE)
    {
        HANDLE_ERROR("error at creating shared memory object");
    }
    if(ftruncate(fd,sizeof(shmem_t))==SYS_FAILURE){

        HANDLE_ERROR("error at extending memory object");
    }
    return fd;
}

void* map_shared_memory(int prot,int flags, int fd, off_t offset){
    
    void *ret = mmap(NULL, sizeof(4), prot, flags, fd, offset);    
    if(ret == MAP_FAILED)
    {
        HANDLE_ERROR("error at mapping shared memory");
    }
    return ret;
}

void unmap_shared_memory(void* addr,size_t len){
    if(munmap(addr, len) == SYS_FAILURE)
    {
        HANDLE_ERROR("error at unmapping memory");
    }
}


void unlink_shared_memory( const char *name){
    if(shm_unlink(name) == SYS_FAILURE)
    {
        HANDLE_ERROR("error with unlink memory");
    }
}

void init_semaphore(sem_t *semaphore, int pshared, unsigned int value){
    if(sem_init(semaphore, pshared, value) == SYS_FAILURE)
    {
        HANDLE_ERROR("error with function init_semaphore");
    }
}

void wait_semaphore(sem_t *semaphore){
    if(sem_wait(semaphore) == SYS_FAILURE)
    {
        HANDLE_ERROR("error with function wait_semaphore");
    }
}

void post_semaphore(sem_t *semaphore){
    if(sem_post(semaphore) == SYS_FAILURE){

        HANDLE_ERROR("error at posting semaphore");
    }
}
void destroy_semaphore(sem_t* semaphore){
    if(sem_destroy(semaphore) == SYS_FAILURE){

        HANDLE_ERROR("error at destroying semaphore");

    }
}
