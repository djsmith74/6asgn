#include "mush.h"

static int c_terminate;

int execute_file () {
    struct sigaction sa;
    /*char *buf[512];*/

    c_terminate = 0;

    sa.sa_handler = &sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

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
    stage_stats **list;
    
    c_terminate = 0;
    
    sa.sa_handler = &sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    sigaction(SIGINT, &sa, NULL);

    while (1) {
        /* parse the command line */
        list = parseline(0);
        printf("post parseline\n");  
        printf("argument: %s\n", list[0]->arg_list[0]);
        /* execute the command */
        exec_main(list); 
    }
    return 0;
}

void sigint_handler(int signum) {
    struct sigaction sa, old;
 
    printf("\nCtrl-C was pressed\n");
    /*printf("8-P ");*/
    fflush(stdout);
    /*sa.sa_handler = SIG_IGN;
    sigaction(SIGINT, &sa, &old);
    kill(0, SIGINT);
    sigaction(SIGINT, &old, NULL);
    while(wait(NULL) != -1) {}
    printf("handler");
    c_terminate = 1;*/
}


