#ifndef MUSH_H
#define MUSH_H

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "exec.h"
#include "pline.h"
#include "main.h"

#define MAX_LINE 512
int execute_file(FILE *file);
int execute_command_line();
void sigint_handler(int signum);

#endif
