#include "main.h"

int main (int argc, char *argv[]) {
    int fd;  
  
    /* Read from standard input */
    if (argc == 1) {  
        printf("only one argument\n");
        execute_command_line(); 
    }
    /* Read from provided file */
    else if (argc == 2) {
        printf("more than one argument\n");
        if((fd = open(argv[1], O_RDONLY)) == -1) {
            perror("bad open");
            exit(EXIT_FAILURE);
        }
        execute_file();
    }
    /* File error due to too many arguments */
    else {
        perror("too many arguments");
        exit(EXIT_FAILURE);
    }

    /* CHANGE THE IO STUFF DAN */

}
