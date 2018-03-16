/*exec.c: opens file descriptors and pipelines
 * also maybe forks for child processes
 */

#include "exec.h"

void exec_main(stage_stats **stats) {
    /*int **fds;
    int i = 0;
    int num;
    char *end;
    int old[2], next[2];*/
    pid_t child, end_id, tpid;
    int child_status;
    int i;
    int list_len;
    char stdin_line[] = "original stdin";
    char stdout_line[] = "original stdout";
    int in_fd, out_fd;
    char dir[50];
    char path[50];
    char *slash = "/";
    char *cmd;
    /*struct stat sb;*/

    list_len = 0;
    i = 0;
    
    while (stats[i] != 0) {
        list_len++; 
        i++;
    }

    printf("list_len: %d\n", list_len);

    i = 0;

    printf("before actual execution\n");
    if (list_len == 1) {
        /*check if cd*/
        if (strcmp(stats[0]->arg_list[0], "cd") == 0) {
            printf("in the chdir\n");
            if (stats[0]->num_args == 2) {
                /*execute the cd*/
                /*if (getcwd(cwd, sizeof(cwd)) == NULL) {
                    perror("getcwd");
                    exit(EXIT_FAILURE);
                }
                strcat(cwd, stats[0]->arg_list[1]);*/
                printf("argument for cd: %s\n", stats[0]->arg_list[1]);
                strcpy(path, stats[0]->arg_list[1]);
                printf("path: %s\n", path);
                strcat(dir, path);
                strcat(dir, "/");
                /*printf("hello\n");*/
                /*if (chdir((stats[0]->arg_list[1])) == -1) {
                    perror(stats[0]->arg_list[1]);
                    exit(EXIT_FAILURE);
                }*/
                printf("dir: %s\n", dir);
                chdir(dir);
                
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
                  /*exit(0);*/
             }
             else {
                while (wait(&child_status) != child) 
                    ;
             }
            /*exit(0);*/
        }
    }
}    
