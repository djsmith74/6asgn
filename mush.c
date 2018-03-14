#include "mush.h"
#include "pline.h"

static int c_terminate;

int execute_file () {
    struct sigaction sa;
    char *buf[512];

    c_terminate = 0;

    sa.sa_handler = &handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL);
    while (1) { /*TODO*/
        /* parse the command line */
        parseline(1);
 
        /* execute the command */
                

        /* check for termination */
        if (c_terminate == 1) {
            printf("HAHAHA\n");
            break;
            /* change this to waiting for all children to die */
        }
    }
    return 0;
}
 

int execute_command_line () {
    struct sigaction sa;

    c_terminate = 0;
    
    sa.sa_handler = &handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL);

    while (1) {
        /* parse the command line */
        parseline(0);
 
        /* execute the command */
                

        /* check for termination */
        if (c_terminate == 1) {
            printf("HAHAHA\n");
            break;
            /* change this to waiting for all children to die */
        }
    }
    return 0;
}

void handler(int signum) {
    printf("\nCtrl-C was pressed\n");
    c_terminate = 1;
}
