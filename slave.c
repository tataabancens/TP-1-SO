#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 4000

void doTask(char* task);

int main (int argc, char* argv[]){

    doTask(argv[1]);

    char* task;
    int dim;
    while((dim = read(STDIN_FILENO, task, BUFFER_SIZE)) != 0){
        if(dim == -1){
            printf("Error reading from pipe");
        }
        task[dim] = '\0';
        doTask(task);
    }
    return 0;
}

void doTask(char* task){
    char command[BUFFER_SIZE + 1];
    char output[BUFFER_SIZE + 1]; 
    sprintf(command, "%s %s | grep -o -e \"Number of.*[0-9]\\+\" -e \"CPU time.*\" -e \".*SATISFIABLE\"", "minisat", task);
    FILE *stream;
    if((stream = popen(command, "r")) == NULL)
    {
        printf("Error en popen");
    }
    int dim = fread(output, sizeof(char), BUFFER_SIZE, stream);

    output[dim] = 0;

    printf("PID: %d Filename: %s\n%s", getpid(), task, output);

    if(pclose(stream) == -1){
        printf("Error en pclose");
    }
}