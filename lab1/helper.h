#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

//#include "parse.h"

#define PIPE_IN		0
#define PIPE_OUT	1

void readPathEnv();
void execPgm(Command *cmd);
void execRecursive(Pgm *pgm, int out);
void execChain(Command *cmd);
