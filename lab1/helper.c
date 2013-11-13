#include "helper.h"


void readPathEnv() {
	char *dirs = getenv("PATH");
	char str[] = "lol:foo:bar";

	char * pch;

	pch = strtok(str, ":");

	while(pch != NULL)
	{
		printf("%s\n", pch);
		pch = strtok(NULL, ":");
	}
}