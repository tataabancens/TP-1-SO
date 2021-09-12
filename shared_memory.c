#include "shared_memory.h"

shmem_t createSharedMem(char* name, int size) {

    shmem_t toRet;
    toRet.rIndex = 0;
    toRet.wIndex = 0;
    toRet.size = size;
    strcpy(toRet.name, name);

    toRet.fd = shm_open(SHRD_MEM_OBJ, O_CREAT | O_RDWR, S_IWUSR);

    if (toRet.fd == SYS_FAILURE) {
        HANDLE_ERROR("error at creating shared memory object");
    }
    if (ftruncate(toRet.fd, size) == SYS_FAILURE) {
        HANDLE_ERROR("error at extending memory object");
    }

    toRet.address = mmap(NULL, sizeof(4), PROT_WRITE | PROT_READ, MAP_SHARED, toRet.fd, 0);    
    if(toRet.address == MAP_FAILED) {
        HANDLE_ERROR("error at mapping shared memory");
    }

    return toRet;
}

void writeSharedMem(shmem_t *shmem, char* buffer, int size, int offset) {
    memcpy(shmem->address + shmem->wIndex, buffer, size + 1);
    shmem->wIndex += size + offset;
}

void deleteSharedMem(shmem_t *shmem) {

    int fd = shmem->fd;
    char *name = shmem->name;
    if (munmap(shmem->address, shmem->size) == SYS_FAILURE) {
        HANDLE_ERROR("error at unmapping memory");
    }
    if (shm_unlink(name) == SYS_FAILURE) {
        HANDLE_ERROR("error with unlink memory");
    }
    if (close(fd) == -1) {
        HANDLE_ERROR("Error in close shm");
    }
}

shmem_t joinSharedMem(char* name, int size) {

    shmem_t toRet;
    strcpy(toRet.name, name);
    toRet.rIndex = 0;
    toRet.rIndex = 0;
    toRet.size = size;
    if ((toRet.fd = (shm_open(name, O_RDONLY, 0))) == -1) {
        HANDLE_ERROR("Error opening shmem in vision");
    }    
    if((toRet.address = (mmap(NULL, sizeof(4), PROT_READ, MAP_SHARED, toRet.fd, 0))) == MAP_FAILED) {
        HANDLE_ERROR("error at mapping shared memory");
    }

    return toRet;
}
char *readSharedMem(shmem_t* shmem) {
    char* toRet = shmem->address + shmem->rIndex;
    int size = strlen(toRet);
    shmem->rIndex += size + 3;
    return toRet;
}

void closeSharedMed(shmem_t *shmem) {
    int fd = shmem->fd;
    if (munmap(shmem->address, shmem->size) == SYS_FAILURE) {
        HANDLE_ERROR("error at unmapping memory");
    }
    if (close(fd) == -1) {
        HANDLE_ERROR("Error in close shm");
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
