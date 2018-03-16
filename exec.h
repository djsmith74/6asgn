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

void exec_main(stage_stats **stats); /*, char *new_buff);*/ 

#endif
