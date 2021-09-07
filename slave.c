#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main (int argc, char* argv[]){
    char command[500];
    char output[500];
    sprintf(command, "%s %s | grep -o -e \"Number of.*[0-9]\\+\" -e \"CPU time.*\" -e \".*SATISFIABLE\"", "minisat", argv[1]);
    FILE *stream;
    if((stream = popen(command, "r")) == NULL)
    {
        printf("Error en popen");
    }
    int dim = fread(output, sizeof(char), 499, stream);

    output[dim] = 0;

    printf("PID: %d Filename: %s\n%s", getpid(), argv[1], output);

    if(pclose(stream) == -1){
        printf("Error en pclose");
    }
    return 0;
}