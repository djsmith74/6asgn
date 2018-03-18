#include "mush.h"
#include "io.h"

static int c_terminate;

int execute_file (FILE *file) {
    struct sigaction sa;
    stage_stats **list;
    int max;
    char *in_line = calloc(MAX_LINE_LEN+5, sizeof(char));
    char **buffer = calloc(MAX_LINE_LEN+1, sizeof(char*));
    char *new_buff = calloc(MAX_LINE_LEN+1, sizeof(char));
    char *pos;

    c_terminate = 0;

    sa.sa_handler = &sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    sigaction(SIGINT, &sa, NULL);
    printf("haters dab back\n");
    

    while (fgets(in_line, MAX_LINE_LEN+5, file) != NULL) {
            /*remove newline character*/
            if ((pos = strchr(in_line, '\n')) != NULL) {
                *pos = '\0';
            }
            
            max = divide_line(in_line, buffer);
            list = parsing(in_line, buffer, max); 
        
            exec_main(list);
    }

    return 0;
}
 

int execute_command_line () {
    struct sigaction sa;
    stage_stats **list;
    int max;
    char *in_line = calloc(MAX_LINE_LEN+5, sizeof(char));
    char **buffer = calloc(MAX_LINE_LEN+1, sizeof(char*));
    char *new_buff = calloc(MAX_LINE_LEN+1, sizeof(char));

    c_terminate = 0;
    
    sa.sa_handler = &sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    sigaction(SIGINT, &sa, NULL);

    while (1) {

        get_input(in_line);
        strncpy(new_buff, in_line, MAX_LINE);
        max = divide_line(in_line, buffer);

        list = parsing(in_line, buffer, max);

        /* execute the command */
        exec_main(list); 
    }
    return 0;
}

void sigint_handler(int signum) {
    struct sigaction sa, old;
    /*stage_stats **list;
    int max;
    char *in_line = calloc(MAX_LINE_LEN+5, sizeof(char));
    char **buffer = calloc(MAX_LINE_LEN+1, sizeof(char*));
    char *new_buff = calloc(MAX_LINE_LEN+1, sizeof(char));*/

 
    printf("\n8-P ");
    fflush(stdout);
    first = 0;
 
    sa.sa_handler = &sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    sigaction(SIGINT, &sa, NULL);

    /*while (1) {

        get_input(in_line);
        strncpy(new_buff, in_line, MAX_LINE);
        max = divide_line(in_line, buffer);

        list = parsing(in_line, buffer, max);

        printf("argument: %s\n", list[0]->arg_list[0]);
        exec_main(list); 
    }*/
}

