#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "parse.h"

typedef struct l {
	char *pgmName;
	char *pgmPath;
	struct l *next;
} Program;