#include "shared_memory.h"

shmem_t createSharedMem(char* name, int size) {

    shmem_t toRet;
    toRet.rIndex = 0;
    toRet.wIndex = 0;
    strcpy(toRet.name, name);

    int fd = shm_open(SHRD_MEM_OBJ, O_CREAT | O_RDWR, S_IWUSR);

    if (fd == SYS_FAILURE) {
        HANDLE_ERROR("error at creating shared memory object");
    }
    if (ftruncate(fd, size) == SYS_FAILURE) {
        HANDLE_ERROR("error at extending memory object");
    }

    toRet.address = mmap(NULL, sizeof(4), PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);    
    if(toRet.address == MAP_FAILED) {
        HANDLE_ERROR("error at mapping shared memory");
    }

    return toRet;
}

void writeSharedMem(shmem_t *shmem, char* buffer, int size, int offset) {
    memcpy(shmem->address + shmem->wIndex, buffer, size + 1);
    shmem->wIndex += size + offset;
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
