/*exec.c: opens file descriptors and pipelines
 * also maybe forks for child processes
 */

#include "exec.h"


void exec_main(stage_stats **stats, char *new_buff) {
    int **fds;
    int i = 0;
    int num;
    char *end;
    int old[2], next[2];
    pid_t child;
    
    while (stats[i] != NULL) {
        open_fds(stats[i]);
        execute(stats[i]);
    }

    /*telephone*/
    if (pipe(old)) {
        perror("old pipe");
        exit(EXIT_FAILURE);
    }
    write(old[WRITE_END], MSG, strlen(MSG));

    for (i = 0; i < num; i++) {
        if (i < num - 1) {   /*create new pipe*/
            if (pipe(next)) {
                perror("next pipe");
                exit(EXIT_FAILURE);
            }
        }
        if (!(child = fork())) {
            /*child*/
            if (dup2(old[READ_END], STDIN_FILENO) == -1) {
                perror("dup2 old");
            }

            if (i < num - 1) {
                if (dup2(next[WRITE_END], STDOUT_FILENO) == -1) {
                    perror("dup2 new");
                }
            }

            close(old[0]);
            close(old[1]);
            close(next[0]);
            close(next[1]);
            telephone(i);
            exit(EXIT_SUCCESS);
        }
        /*parent*/
        /*close up old pipe*/
        close(old[0]);
        close(old[1]);
        old[0] = next[0];
        old[1] = next[1];
    }

    while (num--) {
        if (wait(NULL) == -1) {
            perror("wait");
        }
    }
    return 0;
}
 
}


int open_fds(stat) {
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
