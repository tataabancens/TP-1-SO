#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>

#define SHRD_MEM_OBJ "/myMemObj"
#define BLOCK_SIZE 4096
#define SYS_FAILURE (-1)

int open_shared_mem_object(int oflag, mode_t mode);
void extend_memory_object(int fd, int size);
void* map_shared_memory(void *addr,size_t length,int prot,int flags, int fd, off_t offset);
void init_semaphore(sem_t *semaphore, int pshared, unsigned int value);
void wait_semaphore(sem_t *semaphore);
void post_semaphore(sem_t *semaphore);
void destroy_semaphore(sem_t* semaphore);

#define HANDLE_ERROR(msg)   \
    do                      \
    {                       \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while (0)             \

#endif