#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

//#include "parse.h"

#define PIPE_IN		1
#define PIPE_OUT	0

void readPathEnv();
void execPgm(Command *cmd);
int execRecursive(Pgm *pgm, int pipes_in, int pipes_out);
void execChain(Command *cmd);
