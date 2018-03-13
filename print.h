#ifndef PRINT_H
#define PRINT_H

#include <stdio.h>
#include <stdlib.h>
#include "pline.h"

#define MAX_COMMANDS 10

void print(stage_stats **stats, char *buffer);

void print_stats(int i, stage_stats *stg, char *line);

#endif
