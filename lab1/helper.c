
#include "parse.h"
#include "helper.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void execPgm(Command *cmd) {

	//User wants to exit the bash.
	if(strcmp(cmd->pgm->pgmlist[0], "exit") == 0){
    	printf("Exiting bash..\n");
        exit(0);
    }
    //Built-in function 'cd'.
    else if(strcmp(cmd->pgm->pgmlist[0], "cd") == 0){
        if(chdir(cmd->pgm->pgmlist[1]) < 0){
        	printf("ERROR: %s\n", strerror(errno));
        }
        return;
    }
	pid_t child_pid;

	child_pid = fork();
	if( child_pid == 0 )
	{
		if(cmd->bakground) {
		//Ignore interrupt signals for children in background.
		signal(SIGINT, SIG_IGN);
		}
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

		if(cmd->pgm->next != NULL)
		{
			execRecursive(cmd->pgm, -1, -1);
			exit(0);
		}

		//Start the binaries in the child.
		int isError = execvp(*cmd->pgm->pgmlist, cmd->pgm->pgmlist);
		if( isError < 0 ) {
			printf("ERROR: %s\n", strerror(errno));
			exit(0);
		}
	} 
	if(cmd->bakground){
		//If child is run in background, do not wait for its termination.
		return;
	}
	waitpid(child_pid, NULL, 0);
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

			if(execvp(*pgm->pgmlist, pgm->pgmlist) < 0){
				printf("ERROR: %s\n", strerror(errno));
				exit(0);
			}
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

			if(execvp(*pgm->pgmlist, pgm->pgmlist) < 0){
				printf("ERROR: %s\n", strerror(errno));
				exit(0);
			}
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


			if(execvp(*pgm->pgmlist, pgm->pgmlist) < 0){
				printf("ERROR: %s\n", strerror(errno));
				exit(0);
			}

		} else {
			close(pipes[PIPE_OUT]);
			close(pipes[PIPE_IN]);
			wait(NULL);
		}
	}
}
