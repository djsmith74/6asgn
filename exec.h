#ifndef EXEC_H
#define EXEC_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#include <libgen.h>
#include <fcntl.h>
#include "pline.h"

#define READ_END 0
#define WRITE_END 1

void exec_main(stage_stats **stats); 
int cd(char *path);
int exec_command(stage_stats **stats);
int exec_single_pipe(stage_stats **stats);
int exec_two_pipes(stage_stats **stats);
int exec_more_pipes(stage_stats **stats, int l_len);
int exec_pipes(stage_stats **stats, int l_len);

#endif
