#ifndef PLINE_H
#define PLINE_H

#include <string.h>

#define MAX_ARGS 10
#define MAX_LINE_LEN 512
#define MAX_COMMANDS 10

/*typedef struct stage stage_stats;

struct stage {
    char **c_line;
    char *input_line;
    char *output_line;
    int num_args;
    char **arg_list;
};*/

typedef struct stage {
    char *input_line;
    char *output_line;
    int num_args;
    char **arg_list;
} stage_stats;

stage_stats **parseline(FILE *file);
stage_stats **parsing(char *in_line, char **buffer, int max);
stage_stats *createStage(char *input, char *output, int num_args,
                         char *arg_list[]);
                
#endif
