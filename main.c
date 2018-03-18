#include "main.h"

int main (int argc, char *argv[]) {
    int fd;
    FILE *file;
    char buf[MAX_PATH];
  
    /* Read from standard input */
    if (argc == 1) {  
        execute_command_line(); 
    }
    /* Read from provided file */
    else if (argc == 2) {
        if ((file = fopen(argv[1], "r")) == NULL) {
            perror(argv[1]);
            exit(EXIT_FAILURE);
        }
        execute_file(file);
    }
    /* File error due to too many arguments */
    else {
        perror("Too many arguments");
        exit(EXIT_FAILURE);
    }

    return 0;
}
