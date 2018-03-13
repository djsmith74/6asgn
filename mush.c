#include "mush.h"
#include "pline.h"

static int c_terminate;

int main(int argc, char *argv[]) {
    struct sigaction sa;

    c_terminate = 0;
    
    sa.sa_handler = &handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL);

    while (pause() /* haven't gotten the interrupt signal (^C) */) {
        /* parse the command line */
        /*parseline (argc, argv);*/ 
 
        /* execute the command */
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
