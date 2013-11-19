
#include "parse.h"
#include "helper.h"

/*
void readPathEnv() {
	char *dirs = getenv("PATH");
	char str[] = "lol:foo:bar";

	char * pch;

	char buffer[256];
	sprintf(buffer, "%s", pch);

	pch = strtok(buffer, ":");

	while(pch != NULL)
	{
		printf("%s\n", pch);
		pch = strtok(NULL, ":");
	}

	//printf("%s", dirs);
	//printf("%s\n", foo);
}*/

void execPgm(Command *cmd) {
	if(strcmp(cmd->pgm->pgmlist[0], "exit") == 0){
    	printf("Exiting bash..\n");
        exit(0);
    }
    else if(strcmp(cmd->pgm->pgmlist[0], "cd") == 0){
        if(chdir(cmd->pgm->pgmlist[1]) < 0){
        	printf("ERROR: %s\n", strerror(errno));
        }
        return;
    }
	pid_t child_pid;

	if(cmd->pgm->next != NULL)
	{
		//printf("%s\n", "Executing pipes commands");
		execRecursive(cmd->pgm, -1);
		wait(NULL);
	}

	child_pid = fork();
	if( child_pid == 0 )
	{
		int isError = execvp(*cmd->pgm->pgmlist, cmd->pgm->pgmlist);

		if( isError < 0 ) {
			printf("ERROR: %s\n", strerror(errno));
		}
	} if(cmd->bakground) {
		//Take care of the child when it terminates.
		signal(SIGCHLD, SIG_IGN);
		return;
		//printf("%s\n", "done");
	}
	wait(NULL);
}

void execRecursive(Pgm *pgm, int pipe_in) // out=-1 for first run
{
	int pipes[2];

	if(pgm->next == NULL) 	// We're the last one. Keep stdin.
	{
		printf("Last: %s\n", *pgm->pgmlist);
		if(fork() == 0)
		{
			dup2(pipe_in, fileno(stdout)); // Replace stdout
			close(pipe_in);

			execvp(*pgm->pgmlist, pgm->pgmlist);
			// Implement error checking
		} else {
			return; // Parent will return; no more recursive calls
		}
	}
	else if(pipe_in == -1) 	// We're the first one. Keep stdout
	{
		//setup pipes
		pipe(pipes);

		printf("First: %s\n", *pgm->pgmlist);
		printf("stdin:%d, PIPE_IN:%d\n", fileno(stdin), pipes[PIPE_IN]);

		execRecursive(pgm->next, pipes[PIPE_IN]);

		if(fork() == 0)
		{
			dup2(pipes[PIPE_OUT], fileno(stdin));

			close(pipes[PIPE_OUT]);
			close(pipes[PIPE_IN]);

			execvp(*pgm->pgmlist, pgm->pgmlist);
			// Implement error checking
		} else {
			// Close pipes in parent
			close(pipes[PIPE_OUT]);
			close(pipes[PIPE_IN]);

			return;
		}


	} else { // In the middle, replace both stdin and stdout.
		//TODO: Implement
		pipe(pipes);

		printf("Middle: %s\n", *pgm->pgmlist);

		execRecursive(pgm->next, pipes[PIPE_IN]);

		if(fork() == 0)
		{
			dup2(pipe_in, fileno(stdout));
			dup2(pipes[PIPE_OUT], fileno(stdin));
			close(pipe_in);
			close(pipes[PIPE_OUT]);
			close(pipes[PIPE_IN]);

			execvp(*pgm->pgmlist, pgm->pgmlist);
		} else {
			close(PIPE_OUT);
			close(PIPE_IN);
		}
	}
}
