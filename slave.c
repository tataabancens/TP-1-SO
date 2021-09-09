#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 4000

void doTask(char task[]);

int main(int argc, char* argv[]) {
    
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stdin, 0, _IONBF, 0);

    doTask(argv[1]);

    char task[BUFFER_SIZE + 1] = {0};
    int dimRead;

    while ((dimRead = read(STDIN_FILENO, task, BUFFER_SIZE)) != 0) { 
        if (dimRead == -1) {
            printf("Error in read");
        }
        task[dimRead] = 0;
        doTask(task);
    }

    return 0;
}

void doTask(char task[]){

    char command[500];
    char output[500];
    sprintf(command, "%s %s | grep -o -e \"Number of.*[0-9]\\+\" -e \"CPU time.*\" -e \".*SATISFIABLE\"", "minisat", task);
    
    FILE *stream;
    if ((stream = popen(command, "r")) == NULL) {
        printf("Error en popen");
    }
    
    int dim = fread(output, sizeof(char), 499, stream);
    output[dim] = 0;

    printf("PID: %d Filename: %s\n%s", getpid(), task, output);

    if (pclose(stream) == -1) {
        printf("Error en pclose");
    }
}