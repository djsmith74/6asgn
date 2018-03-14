#include "main.h"
#include "mush.h"

int main (int argc, char *argv[]) {
    int fd;  
  
    /* Read from standard input */
    if (argc == 1) { 
        execute_command_line(); 
    }
    /* Read from provided file */
    else if (argc == 2) {
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
