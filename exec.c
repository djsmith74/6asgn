/*exec.c: opens file descriptors and pipelines
 * also maybe forks for child processes
 */

#include "exec.h"

void exec_main(stage_stats **stats) {
    pid_t child;
    int child_status;
    int i;
    int list_len;
    char *cmd;
    
    list_len = 0;
    i = 0;
    
    while (stats[i] != 0) {
        list_len++; 
        i++;
    }
    printf("number of stages: %d\n", list_len);

    printf("list_len: %d\n", list_len);

    i = 0;
    
    printf("before actual execution\n");
    if (list_len == 1) {
        /*check if cd*/
        if (strcmp(stats[0]->arg_list[0], "cd") == 0) {
            printf("in the chdir\n");
            if (stats[0]->num_args == 2) {
                cd(stats[0]->arg_list[1]); 
            }
            else {
                printf("usage: cd <directory>\n");
            }
        }
        else {
             printf("trying to execute \n"); 
             cmd = stats[i]->arg_list[0];
             printf("command: '%s'\n", cmd);
             child = fork();
             if (child == -1) {
                 perror("fork");
                 exit(EXIT_FAILURE);
             }
             else if (child == 0) {
                 exec_command(stats);       
             }
             else {
                while (wait(&child_status) != child) 
                    ;
             }
        }
    }
    else if (list_len == 2) {
        exec_single_pipe(stats);          
    }
    else if (list_len > 2) {
        exec_pipes(stats, list_len);
    }
    else {
        perror("pipes");
        exit(EXIT_FAILURE);
    }
}    

int cd(char *path) {
    char *dir = calloc(50, sizeof(char)); /*get rid of magic*/
    char *path_cp = calloc(50, sizeof(char));
     
    /*execute the cd*/
    /*if (getcwd(cwd, sizeof(cwd)) == NULL) {
          perror("getcwd");
          exit(EXIT_FAILURE);
      }
      strcat(cwd, stats[0]->arg_list[1]);*/
      /*printf("argument for cd: %s\n", stats[0]->arg_list[1]);*/
      strcpy(path_cp, path);
      /*printf("path: %s\n", path);*/
      strcat(dir, path_cp);
      strcat(dir, "/");
      /*printf("hello\n");*/
      /*if (chdir((stats[0]->arg_list[1])) == -1) {
          perror(stats[0]->arg_list[1]);
          exit(EXIT_FAILURE);
       }*/
      printf("dir: %s\n", dir);
      chdir(dir);
      return 0;          
}

int exec_command(stage_stats **stats) {
     int in_fd, out_fd;
     int i;

     i = 0;
     if ( stats[i]->input_line != NULL ) {
         in_fd = open(stats[i]->input_line, O_RDONLY);
         dup2(in_fd, STDIN_FILENO);
     } 
     if ( stats[i]->output_line != NULL ) {
         out_fd = open(stats[i]->output_line, O_CREAT | O_WRONLY);
         dup2(out_fd, STDOUT_FILENO);
     }
     printf("arg: '%s'\n", stats[i]->arg_list[0]);
     printf("arg_num: %d\n", stats[i]->num_args);
     execvp(stats[i]->arg_list[0], stats[i]->arg_list);
     fflush(stdout);
     return 0;
}

int exec_single_pipe(stage_stats **stats) {
    int in_fd, out_fd;
    int child_status;
    int one[2] = {0};
    int i;
    pid_t child1, child2;
     
    if ( pipe(one) ) { 
        perror("First pipe"); 
        exit(-1);
    }

    if (!(child1 = fork())) {
        i = 0;
        /* child one stuff */
        /* dup appropriate pipe ends */
        printf("input line: %s\n", stats[i]->input_line);
        if ( stats[i]->input_line != NULL ) {
            in_fd = open(stats[i]->input_line, O_RDONLY);
            dup2(in_fd, STDIN_FILENO);
            printf("love you :)\n");
        } 
        printf("pre dupdup\n");
        printf("stdout_fileno: %d\n", STDOUT_FILENO);
        if (-1 == dup2(one[WRITE_END], STDOUT_FILENO)) {
            perror("dup2"); 
            exit(-1);
        }
        printf("pre cleanup\n");
         
        /* clean up */ 
        close(one[READ_END]); 
        close(one[WRITE_END]); 
        printf("post cleanup\n");

        /* do the exec */ 
        execvp(stats[i]->arg_list[0], stats[i]->arg_list); 
        fflush(stdout);
        perror(stats[i]->arg_list[0]);
        exit(-1);
    }
 
    if (!(child2 = fork())) {
        i = 1;
        /* child two stuff */
        if ( stats[i]->output_line != NULL ) {
            out_fd = open(stats[i]->output_line, O_CREAT | O_WRONLY);
            dup2(out_fd, STDOUT_FILENO);
        }

        if (-1 == dup2(one[READ_END], STDIN_FILENO)) {    
            perror("dup2");
            exit(-1); 
        }

        /* clean up */ 
        close(one[READ_END]); 
        close(one[WRITE_END]);

        /* do the exec */ 
        execvp(stats[i]->arg_list[0], stats[i]->arg_list); 
        fflush(stdout);
        perror(stats[i]->arg_list[0]);
        exit(-1);
    } 
    close(one[READ_END]);
    close(one[WRITE_END]);

    if (-1 == wait(NULL)) {
        perror("wait");
    }
    if (-1 == wait(NULL)) {
        perror("wait");
    }
    return 0;
}

int exec_pipes(stage_stats **stats, int l_len) {
    int in_fd, out_fd;
    int child_status;
    int num_children;
    int one[2] = {0};
    int i;
    pid_t child1, child2;
    int k;
    int fd[2];
    int readfd[num_children];
    int writefd[num_children];    
    int procID[num_children];
 
    num_children = l_len - 2;

    if ( pipe(one) ) { 
        perror("First pipe"); 
        exit(-1);
    }

    if (!(child1 = fork())) {
        i = 0;
        /* child one stuff */
        /* dup appropriate pipe ends */
        printf("input line: %s\n", stats[i]->input_line);
        if ( stats[i]->input_line != NULL ) {
            in_fd = open(stats[i]->input_line, O_RDONLY);
            dup2(in_fd, STDIN_FILENO);
            printf("love you :)\n");
        } 
        printf("pre dupdup\n");
        printf("stdout_fileno: %d\n", STDOUT_FILENO);
        if (-1 == dup2(one[WRITE_END], STDOUT_FILENO)) {
            perror("dup2"); 
            exit(-1);
        }
        printf("pre cleanup\n");
         
        /* clean up */ 
        close(one[READ_END]); 
        close(one[WRITE_END]); 
        printf("post cleanup\n");

        /* do the exec */ 
        execvp(stats[i]->arg_list[0], stats[i]->arg_list); 
        fflush(stdout);
        perror(stats[i]->arg_list[0]);
        exit(-1);
    }
 
    while(i < (l_len-1)) {
        if (pipe(fd)) {
            perror("pipe");
            exit(-1);
        }
        /* Storing the pipe ID */
        readfd[i] = fd[0];
        writefd[i] = fd[1];
        /*if (procID[i] = fork() == -1) {
            perror("bad fork")
            exit(EXIT_FAILURE);
        } 
        if (procID[i] == 0) {
            
            dup2(writefd[i], STDOUT_FILENO);
        }*/
        if (!(procID[i] = fork())) {
            /*if (-1 == dup2(readfd[i], STDIN_FILENO)) {
                perror("dup2");
                exit(-1);
            }*/
            if (-1 == dup2(writefd[i], STDOUT_FILENO)) {
                perror("dup2");
                exit(-1);
            }

            /* clean up */
            close(readfd[i]);

            /* do the exec */
            execvp(stats[i]->arg_list[0], stats[i]->arg_list);
            fflush(stdout);
            perror(stats[i]->arg_list[0]);
            exit(-1);
        }
        i++;   
    }
  
    if (!(child2 = fork())) {
        i = 1;
        /* child two stuff */
        if ( stats[i]->output_line != NULL ) {
            out_fd = open(stats[i]->output_line, O_CREAT | O_WRONLY);
            dup2(out_fd, STDOUT_FILENO);
        }

        if (-1 == dup2(one[READ_END], STDIN_FILENO)) {    
            perror("dup2");
            exit(-1); 
        }

        /* clean up */ 
        close(one[READ_END]); 
        close(one[WRITE_END]);

        /* do the exec */ 
        execvp(stats[i]->arg_list[0], stats[i]->arg_list); 
        fflush(stdout);
        perror(stats[i]->arg_list[0]);
        exit(-1);
    } 
    close(one[READ_END]);
    close(one[WRITE_END]);

    if (-1 == wait(NULL)) {
        perror("wait");
    }
    if (-1 == wait(NULL)) {
        perror("wait");
    }    
    return 0;
}

