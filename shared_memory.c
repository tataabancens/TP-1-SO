#include "shared_memory.h"

shmem_t createSharedMem(char* name, int size) {

    shmem_t toRet;
    toRet.rIndex = 0;
    toRet.wIndex = 0;
    toRet.size = size;
    strcpy(toRet.name, name);

    toRet.fd = shm_open(name, O_CREAT | O_RDWR, 0666);

    if (toRet.fd == SYS_FAILURE) {
        HANDLE_ERROR("error at creating shared memory object");
    }
    if (ftruncate(toRet.fd, size) == SYS_FAILURE) {
        HANDLE_ERROR("error at extending memory object");
    }

    toRet.address = mmap(NULL, toRet.size, PROT_WRITE | PROT_READ, MAP_SHARED, toRet.fd, 0);    
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
    char *address = shmem->address;
    int size = shmem->size;
    printf("1");
    if (munmap(address, size) == SYS_FAILURE) {
        HANDLE_ERROR("error at unmapping memory");
    }
    printf("2");
    if (shm_unlink(name) == SYS_FAILURE) {
        HANDLE_ERROR("error with unlink memory");
    }
    printf("3");
    if (close(fd) == -1) {
        HANDLE_ERROR("Error in close shm");
    }
    printf("4");
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
    if((toRet.address = (mmap(NULL, toRet.size, PROT_READ, MAP_SHARED, toRet.fd, 0))) == MAP_FAILED) {
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


t_sem openSem(char* name) {
    t_sem toRet;
    strcpy(toRet.name, name);
    toRet.access = sem_open(name, O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH, 0);
    if (toRet.access == SEM_FAILED) {
        HANDLE_ERROR("error with open sem");
    }
    return toRet;
}

void waitSem(t_sem *sem) {
    if(sem_wait(sem->access) == SYS_FAILURE) {
        HANDLE_ERROR("error with function wait_semaphore");
    }
}

void postSem(t_sem *sem) {
    if(sem_post(sem->access) == SYS_FAILURE) {
        HANDLE_ERROR("error at posting semaphore");
    }
}

void closeSem(t_sem *sem) {
    if (sem_close(sem->access) == SYS_FAILURE) {
        HANDLE_ERROR("error at closing sem");
    }
}

void destroySem(t_sem* sem) {
    if(sem_close(sem->access) == SYS_FAILURE) {
        HANDLE_ERROR("error at closing sem");
    }
    if (sem_unlink(sem->name) == SYS_FAILURE) {
        HANDLE_ERROR("error at unlinking sem");
    }
}
