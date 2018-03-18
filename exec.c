/*exec.c: opens file descriptors and pipelines
 * also maybe forks for child processes
 */

#include "exec.h"

void exec_main(stage_stats **stats) {
    int i;
    int list_len;
    
    list_len = 0;
    i = 0;
    
    while (stats[i] != 0) {
        list_len++; 
        i++;
    }

    i = 0;
    
    if (list_len == 1) {
        /*check if cd*/
        if (strcmp(stats[0]->arg_list[0], "cd") == 0) {
            if (stats[0]->num_args == 2) {
                cd(stats[0]->arg_list[1]); 
            }
            else {
                printf("usage: cd <directory>\n");
            }
        }
        else {
             /*child = fork();
             if (child == -1) {
                 perror("fork");
                 exit(EXIT_FAILURE);
             }
             else if (child == 0) {*/
                 exec_command(stats);       
             /*}
             else {
                while (wait(&child_status) != child) 
                    ;
             }*/
        }
    }
    else if (list_len == 2) {
        exec_single_pipe(stats);          
    }
    else if (list_len == 3) {
        exec_two_pipes(stats);
    }
    else if (list_len > 3) {
        exec_more_pipes(stats, list_len);
    }
    else {
        perror("pipes");
        exit(EXIT_FAILURE);
    }
}    

int cd(char *path) {
    char *dir = calloc(DIR_SIZE, sizeof(char)); 
    char *path_cp = calloc(PATH_SIZE, sizeof(char));
     
    strcpy(path_cp, path);
    strcat(dir, path_cp);
    strcat(dir, "/");
    chdir(dir);
    return 0;          
}

int exec_command(stage_stats **stats) {
     int in_fd, out_fd;
     int i;
     pid_t child1;

     i = 0;
     if (!(child1 = fork())) { 
         if ( stats[i]->input_line != NULL ) {
             in_fd = open(stats[i]->input_line, O_RDONLY);
             dup2(in_fd, STDIN_FILENO);
         
         } 
         if ( stats[i]->output_line != NULL ) {
             out_fd = open(stats[i]->output_line, O_CREAT | O_WRONLY | O_TRUNC, 0666);
             dup2(out_fd, STDOUT_FILENO);
         }

         execvp(stats[i]->arg_list[0], stats[i]->arg_list);
         exit(-1);
     }
     /*else {
         while (wait(&child_status) != child1) 
             ;
     }*/
     if (-1 == wait(NULL)) {
         perror("wait");
     }
     return 0;
}

int exec_single_pipe(stage_stats **stats) {
    int in_fd, out_fd;
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
        if ( stats[i]->input_line != NULL ) {
            in_fd = open(stats[i]->input_line, O_RDONLY);
            dup2(in_fd, STDIN_FILENO);
        } 
        if (-1 == dup2(one[WRITE_END], STDOUT_FILENO)) {
            perror("dup2"); 
            exit(-1);
        }
         
        /* clean up */ 
        close(one[READ_END]); 
        close(one[WRITE_END]); 

        /* do the exec */ 
        execvp(stats[i]->arg_list[0], stats[i]->arg_list); 
        /*fflush(stdout);*/
        perror(stats[i]->arg_list[0]);
        exit(-1);
    }
 
    if (!(child2 = fork())) {
        i = 1;
        /* child two stuff */
        if ( stats[i]->output_line != NULL ) {
            out_fd = open(stats[i]->output_line, O_CREAT | O_WRONLY | O_TRUNC, 0666);
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
        /*fflush(stdout);*/
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

int exec_two_pipes(stage_stats **stats) {
    int one[2];
    int two[2];
    int i;
    pid_t child1, child2, child3;
    int in_fd, out_fd;

    if (pipe(one)) {
        perror("First pipe");
        exit(-1);
    }
    if (pipe(two)) {
        perror("Second pipe");
        exit(-1);
    }
   
    if (!(child1 = fork())) {
        i = 0;
        if (stats[i]->input_line != NULL) {
                in_fd = open(stats[i]->input_line, O_RDONLY);
                if (-1 == dup2(in_fd, STDIN_FILENO)) {
                    perror("dup2");
                    exit(-1);
                }
        } 
        if (-1 == dup2(one[WRITE_END], STDOUT_FILENO)) {
            perror("dup2");
            exit(-1);
        }

        close(one[READ_END]);
        close(one[WRITE_END]);
        close(two[READ_END]);
        close(two[WRITE_END]);
   
        execvp(stats[i]->arg_list[0], stats[i]->arg_list);
        /*fflush(stdout);*/
        perror(stats[i]->arg_list[0]);
        exit(-1);
    }

    if (!(child2 = fork())) {
        i = 1;
        if (-1 == dup2(one[READ_END], STDIN_FILENO)) {
            perror("dup2");
            exit(-1);
        }

        if (-1 == dup2(two[WRITE_END], STDOUT_FILENO)) {
            perror("dup2");
            exit(-1);
        }

        close(one[READ_END]);
        close(one[WRITE_END]);
        close(two[READ_END]);
        close(two[WRITE_END]);

        execvp(stats[i]->arg_list[0], stats[i]->arg_list);
        /*fflush(stdout);*/
        perror(stats[i]->arg_list[0]);
        exit(-1);
    }

    if (!(child3 = fork())) {
        i = 2;
        if (stats[i]->output_line != NULL) {
                out_fd = open(stats[i]->output_line, O_CREAT | O_WRONLY | O_TRUNC, 0666);
                if (-1 == dup2(out_fd, STDOUT_FILENO)) {
                    perror("dup2");
                    exit(-1);
                }
        }
        if (-1 == dup2(two[READ_END], STDIN_FILENO)) {
            perror("dup2");
            exit(-1);
        }

        close(one[READ_END]);
        close(one[WRITE_END]);
        close(two[READ_END]);
        close(two[WRITE_END]);

        execvp(stats[i]->arg_list[0], stats[i]->arg_list);
        perror(stats[i]->arg_list[0]);
        exit(-1);
    }

    close(one[READ_END]);
    close(one[WRITE_END]);
    close(two[READ_END]);
    close(two[WRITE_END]);

    if(-1 == wait(NULL)) {
        perror("wait");
    }
    if(-1 == wait(NULL)) {
        perror("wait");
    }
    if(-1 == wait(NULL)) {
        perror("wait");
    }
    return 0;
}

int exec_more_pipes(stage_stats **stats, int l_len) {
    int one[2];
    int two[2];
    int i;
    int procID[l_len];
    int in_fd, out_fd;
   
    if (pipe(one)) {
        perror("First pipe");
        exit(-1);
    }
    
    i = 0;
    if (!(procID[i] = fork())) {
        i = 0;
        if (stats[i]->input_line != NULL) {
                in_fd = open(stats[i]->input_line, O_RDONLY);
                if (-1 == dup2(in_fd, STDIN_FILENO)) {
                    perror("dup2");
                    exit(-1);
                }
        } 
        if (-1 == dup2(one[WRITE_END], STDOUT_FILENO)) {
            perror("dup2");
            exit(-1);
        }

        close(one[READ_END]);
        close(one[WRITE_END]);
   
        execvp(stats[i]->arg_list[0], stats[i]->arg_list);
        perror(stats[i]->arg_list[0]);
        exit(-1);
    }
    
    for (i = 1; i < l_len; i++) {
        if (i < l_len-1) {
            if(pipe(two)) {
                perror("next pipe");
                exit(EXIT_FAILURE);
            }
        } 
        if (!(procID[i] = fork())) {
            if (-1 == dup2(one[READ_END], STDIN_FILENO)) {
                perror("dup2");
                exit(-1);
            }
            if ((i == l_len-1) && (stats[i]->output_line != NULL)) {
                out_fd = open(stats[i]->output_line, O_CREAT | O_WRONLY | O_TRUNC, 0666);
                if (-1 == dup2(out_fd, STDOUT_FILENO)) {
                    perror("dup2");
                    exit(-1);
                }
            }
            else if (i < l_len-1) {
                if (-1 == dup2(two[WRITE_END], STDOUT_FILENO)) {
                    perror("dup2 new");
                }
            }
        
            close(one[READ_END]);
            close(one[WRITE_END]);
            close(two[READ_END]);
            close(two[WRITE_END]);

            execvp(stats[i]->arg_list[0], stats[i]->arg_list);
            perror(stats[i]->arg_list[0]);
            exit(-1);
        }
        close(one[READ_END]);
        close(one[WRITE_END]);
        one[READ_END] = two[READ_END];
        one[WRITE_END] = two[WRITE_END];
    }
    while (l_len --) {
        if(-1 == wait(NULL)) {
            perror("wait");
        }
    }
    return 0;
}


