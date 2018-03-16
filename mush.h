#ifndef MUSH_H
#define MUSH_H

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include "exec.h"
#include "pline.h"

int execute_file();
int execute_command_line();
void sigint_handler(int signum);

#endif
