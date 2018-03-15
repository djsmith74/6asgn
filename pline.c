#include "io.h"
#include "pline.h"
#include "print.h"

int parseline (int ioflag) {
    int s_num = 0;
    
    /* FLAGS */
    int found_input;
    int found_output;
    int found_pipe;
    int found_in_redir;
    int found_out_redir;
    int found_arg;

    /* ERROR FLAGS */
    int found_amb_in;
    int found_bad_in_redir;
    int found_bad_out_redir;

    /* VARIABLES */
    int max;
    int args_index;
    int stage_num;
    int pipe_num;
    char *line;
    char *input;
    char *output;
    char *tempi;
    char *tempo;
    char **tempa;
    int num_args;
    char *arguments[MAX_ARGS] = {NULL};
    stage_stats *stage_list[MAX_COMMANDS] = {NULL};
    int i;
    int j;
    int k;
    int l;
    int g;
    int struct_index;

    /* INITIATION */
    char stdin_line[] = "original stdin";
    char stdout_line[] = "original stdout";

    char in_line[MAX_LINE_LEN+5] = {0};
    char *buffer[MAX_LINE_LEN];
    char new_buff[MAX_LINE_LEN+1] = {0};

    input = NULL;
    output = NULL;
    tempi = NULL;
    tempo = NULL;
     
    found_input = 0; 
    found_output = 0;
    found_in_redir = 0;
    found_out_redir = 0;
    found_pipe = 0;
    found_arg = 0;

    found_amb_in = 0;
    found_bad_in_redir = 0;
    found_bad_out_redir = 0;

    struct_index = 0;
    args_index = 0;
    num_args = 0;
    pipe_num = 0;
    i = 0;
 
    /* THE START! */
    if (ioflag == 0) {
        get_input(in_line);
        strncpy(new_buff, in_line, MAX_LINE_LEN);
        max = divide_line(in_line, buffer); 
    }
    else {
        /*do nothing*/ 
    }

    while ( i < max ) {
        /* CHECK FLAGS */
        if (found_input) {
            if (pipe_num > 0) {
                found_amb_in = 1;
            }
            else if (strcmp(buffer[i], "<") == 0 || strcmp(buffer[i], ">") == 0
                     || strcmp(buffer[i], "|") == 0) {
                found_bad_in_redir = 1; 
            }
            else if (found_in_redir) {
                found_bad_in_redir = 1;
            }
            else {
                input = buffer[i];
                found_in_redir = 1;
            }
        }
        else if (found_output) {
            if (strcmp(buffer[i], "<") == 0 || strcmp(buffer[i], ">") == 0 ||
                strcmp(buffer[i], "|") == 0) {
                found_bad_out_redir = 1; 
            }
            else if (found_out_redir) {
                found_bad_out_redir = 1; 
            }
            else {
                output = buffer[i];
                found_out_redir = 1;
            }
        }
        else if (found_pipe) {
            if (strcmp(buffer[i], "|") == 0) {
                perror("invalid null command");
                exit(EXIT_FAILURE);
            }
            else if (found_out_redir == 1) {
                printf("%s: ambigious output\n", arguments[0]);
                perror("ambigious output");
                exit(EXIT_FAILURE);
            }
            else {
                if (pipe_num == 0 && input == NULL) {
                    input = stdin_line;
                } 
                if (input == NULL) {
                   tempi = NULL;
                }
                else {
                   tempi = calloc(strlen(input), sizeof(char));
                   strcpy(tempi, input);
                }

                if (output == NULL) {
                   tempo = NULL;
                }
                else { 
                   tempo = calloc(strlen(output), sizeof(char));
                   strcpy(tempo, output);
                }
                if ((tempa = calloc(MAX_ARGS, sizeof(char*))) == NULL) {
                }

                for (g = 0; g < num_args; g++) { 
                    tempa[g] = arguments[g];
                }
                if (num_args == 0) {
                    perror("empty");
                    exit(EXIT_FAILURE);
                }
                stage_list[struct_index] = createStage(tempi, tempo, num_args,
                                                       tempa);
                found_pipe = 0;

                input = NULL;
                output = NULL;
                num_args = 0;
                for (j = 0; j < MAX_ARGS; j++) {
                   arguments[j] = NULL;
                }
                args_index = 0;
                pipe_num++;
                struct_index++;
            }
        }    

        /* SET FLAGS */
        if (strcmp(buffer[i], "<") == 0) {
            found_input = 1;
            if (i == (max-1)) {
                found_bad_in_redir = 1;
            }
        }
        else if (strcmp(buffer[i], ">") == 0) {
            found_output = 1;
            if (i == (max-1)) {
                found_bad_out_redir = 1;
            }
        }
        else if (strcmp(buffer[i], "|") == 0) {
            found_pipe = 1;
        }
        else if (found_input == 1 || found_output == 1) {
            found_arg = 0;
            found_input = 0;
            found_output = 0;
        }
        else {
            found_arg = 1;
        }

        if (found_arg) {
            arguments[args_index] = buffer[i];
            found_arg = 0;
            args_index++;
            num_args++;
            if (num_args > (MAX_ARGS+1)) {
                printf("%s: too many arguments\n", arguments[0]);
                perror("too many arguments");
                exit(EXIT_FAILURE);
            }
        }
 
        if (arguments[0] != NULL) {
            if (found_amb_in == 1) {
                printf("%s: ambigious input\n", arguments[0]);
                perror("ambigious output");
                exit(EXIT_FAILURE);
            }
            if (found_bad_in_redir == 1) {
                printf("%s: bad input redirection\n", arguments[0]);
                perror("bad input redirection"); 
                exit(EXIT_FAILURE);
            }
            if (found_bad_out_redir == 1) {
                printf("%s: bad output redirection\n", arguments[0]);
                perror("bad input redirection");
                exit(EXIT_FAILURE); 
            }
        }
        i++;
    }
    
    if (input == NULL && pipe_num == 0) {
        input = stdin_line;
    }
    if (output == NULL) {
        output = stdout_line;
    }
    if (num_args == 0) {
        perror("empty");
        exit(EXIT_FAILURE);
    }
    stage_list[struct_index] = createStage(input, output, num_args, arguments);

    if (pipe_num > 0) {
        pipe_num++;
        if (pipe_num > MAX_COMMANDS) {
            perror("pipeline too deep\n");
            exit(EXIT_FAILURE);
        }
    }
    
    print(stage_list, new_buff); /*TODO comment this line out*/ 

    /*exec_main(stage_list, new_buff);*/

    return 0;
} 
 
stage_stats *createStage(char *input, char *output, int num_args,
                         char **arg_list) {
    stage_stats *new_stage;
    new_stage = calloc(1, sizeof(stage_stats));
    new_stage->input_line = input;
    new_stage->output_line = output;
    new_stage->num_args = num_args;
    new_stage->arg_list = arg_list;
    return new_stage;
}
