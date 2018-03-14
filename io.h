#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_IN 512


void get_input(char *input);

int divide_line(char *input, char **buffer);


#endif
