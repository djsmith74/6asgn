#include "main.h"

int main (int argc, char *argv[]) {
    int fd;
    FILE *file;
    char buf[MAX_PATH];
  
    /* Read from standard input */
    if (argc == 1) {  
        printf("only one argument\n");
        execute_command_line(); 
    }
    /* Read from provided file */
    else if (argc == 2) {
        printf("more than one argument\n");
        if ((file = fopen(argv[1], "r")) == NULL) {
            perror("bad open");
            printf("hello daberoni i've been expecting you\n");
            exit(EXIT_FAILURE);
        }
        printf("yo yo\n");
        execute_file(file);
    }
    /* File error due to too many arguments */
    else {
        perror("too many arguments");
        exit(EXIT_FAILURE);
    }

    /* CHANGE THE IO STUFF DAN */

}
