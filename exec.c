/*exec.c: opens file descriptors and pipelines
 * also maybe forks for child processes
 */

#include "exec.h"


void exec_main(stage_stats **stats, char *new_buff) {
    int **fds;


    fds = open_fds(stats);
}


int **open_fds(stage_stats **stats) {
    int infd, outfd;
    int fds[2];
    int i = 0;

    while (stats != NULL) {
        if (stats[i]->input != /*stdin*/) {
            open_fd(stats[i]->input);
        }
        else if (stats[i]->output != /*stdout*/) {
            open_fd(stats[i]->output);
        }
        else {
            /*pipe it*/
            
        i++;
    }
}

/*returns a file descriptor*/
int open_fd(char *file) {
    int fd;
}
    


int fork(/*not sure yet*/) {

   pid_t pidc, pid;
   int status = 0;
   char *file;

   pidc = 0;
   pid = 0;

   if (argc < 2 || argc > 2) {
      printf("usage: tryit command\n");
      exit(1);
   }


   if((pid = fork()) < 0) {
      perror("fork");
      exit(EXIT_FAILURE);
   }

   

   if (pid == 0) {
      /*child*/
      file = basename(argv[1]);
      if (execl(argv[1], file, NULL) == -1) {
         perror(argv[1]);
         exit(EXIT_FAILURE);
      }
      else {
         exit(EXIT_SUCCESS);
      }
   }
   else {
      /*parent*/
      while ((pidc = waitpid(pid, &status, WNOHANG|WUNTRACED)) == 0) {
         ;
      }

      if (status > 0) {
         printf("Process %d exited with an error value.\n", pidc);
         exit(EXIT_FAILURE);
      }
      else {
         printf("Process %d succeeded.\n", pidc);
         exit(EXIT_SUCCESS);
      }
   }
   return 0;
}
