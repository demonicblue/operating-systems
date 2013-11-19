
#include "parse.h"
#include "helper.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

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
		execRecursive(cmd->pgm, -1, -1);
		return;
	}

	child_pid = fork();
	if( child_pid == 0 )
	{
		//Replace stdin?
		if(cmd->rstdin != NULL){
			int input_file = 0;
			//Open input file or create it if it doesn't exist.
			if((input_file = open(cmd->rstdin, O_RDONLY | O_CREAT, 00700)) < 0){
				printf("ERROR: %s\n", strerror(errno));
			} 
			dup2(input_file, fileno(stdin)); //Close stdin and use file instead.
			close(input_file); //Close file descriptor.
		}
		//Replace stdout? 	
		if(cmd->rstdout != NULL){
			int output_file = 0;
			//Open output file or create it if it doesn't exist.
			if((output_file = open(cmd->rstdout, O_WRONLY | O_CREAT, 00700)) < 0){
				printf("ERROR: %s\n", strerror(errno));
			}
			dup2(output_file, fileno(stdout)); //Close stdout and use file instead.
			close(output_file); //Close file descriptor.
		}

		//Start the binaries in the child.
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

int execRecursive(Pgm *pgm, int pipe_in, int pipe_out) // out=-1 for first run
{
	int pipes[2];

	if(pgm->next == NULL) 	// We're the last one. Keep stdin.
	{
		int child_pid = fork();
		if(child_pid == 0)
		{
			/*dup2(pipe_in, fileno(stdout)); // Replace stdout
			close(pipe_in);*/

			close(fileno(stdout));
			dup(pipe_in);
			close(pipe_in);
			close(pipe_out);

			execvp(*pgm->pgmlist, pgm->pgmlist);
			// Implement error checking
		} else {
			wait(NULL);
			return child_pid; // Parent will return; no more recursive calls
		}
	}
	else if(pipe_in == -1) 	// We're the first one. Keep stdout
	{
		//setup pipes
		pipe(pipes);

		execRecursive(pgm->next, pipes[PIPE_IN], pipes[PIPE_OUT]);

		int child_pid = fork();

		if(child_pid == 0)
		{
			/*dup2(pipes[PIPE_OUT], fileno(stdin));

			close(pipes[PIPE_OUT]);
			close(pipes[PIPE_IN]);*/

			close(fileno(stdin));
			dup(pipes[PIPE_OUT]);
			close(pipes[PIPE_OUT]);
			close(pipes[PIPE_IN]);

			execvp(*pgm->pgmlist, pgm->pgmlist);
			// Implement error checking
		} else {
			// Close pipes in parent
			close(pipes[PIPE_OUT]);
			close(pipes[PIPE_IN]);
			wait(NULL);
			return child_pid;
		}


	} else { // In the middle, replace both stdin and stdout.
		//TODO: Implement
		pipe(pipes);

		execRecursive(pgm->next, pipes[PIPE_IN], pipes[PIPE_OUT]);

		if(fork() == 0)
		{
			dup2(pipe_in, fileno(stdout));
			dup2(pipes[PIPE_OUT], fileno(stdin));
			close(pipe_in);
			close(pipes[PIPE_OUT]);
			close(pipes[PIPE_IN]);


			execvp(*pgm->pgmlist, pgm->pgmlist);
		} else {
			close(pipes[PIPE_OUT]);
			close(pipes[PIPE_IN]);
			wait(NULL);
		}
	}
}
