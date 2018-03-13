#ifndef MUSH_H
#define MUSH_H

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

int execute_commands();
void handler(int signum);

#endif
