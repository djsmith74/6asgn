#ifndef MUSH_H
#define MUSH_H

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

int execute_file();
int execute_command_line();
void handler(int signum);

#endif
