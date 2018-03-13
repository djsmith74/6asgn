#include "main.h"

int main (int argc, char *argv[]) {
    int fd;  
  
    /* Read from standard input */
    if (argc == 1) {
        
    }
    /* Read from provided file */
    else if (argc == 2) {
        if((fd = open(argv[1], O_RDONLY)) == -1) {
            perror("bad open");
            exit(EXIT_FAILURE);
        }
    }
    /* File error due to too many arguments */
    else {
        perror("too many arguments");
        exit(EXIT_FAILURE);
    }
}
