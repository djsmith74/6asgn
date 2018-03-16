#include "mush.h"

static int c_terminate;

int execute_file () {
    struct sigaction sa;
    /*char *buf[512];*/

    c_terminate = 0;

    sa.sa_handler = &sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL);

    while (1) { /*TODO*/
        /* parse the command line */
        parseline(1);
 
        /* execute the command */
        /*exec_main(*/      
    }
    return 0;
}
 

int execute_command_line () {
    struct sigaction sa;
    struct stage_stats **list;
    
    c_terminate = 0;
    
    sa.sa_handler = &sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL);

    while (1) {
        /* parse the command line */
        list = parseline(0);
 
        /* execute the command */
        exec_main(list);

        /* get the arguments from the struct */ 
    }
    return 0;
}

void sigint_handler(int signum) {
    struct sigaction sa, old;
 
    printf("\nCtrl-C was pressed\n");
    sa.sa_handler = SIG_IGN;
    sigaction(SIGINT, &sa, NULL);
    c_terminate = 1;
}


