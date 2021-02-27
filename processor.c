/*
 *
 * Group 5
 * Major 2
 *
 */

#include "processor.h"

void processor(char* input){
	
	
	char **line = malloc(512 * sizeof(char*)); 					// array of strings to store each command line
	char *delim = ";\n";											// field seperator
	char *parsedInput = strtok(input, delim); 					// holds input parsed at semi-colons 
	int i=0;													// index variable
	
	// parsing input
    while(parsedInput != NULL)
    {
            line[i] = parsedInput;                          	// putting parsed input into the cmd
            i++;

            parsedInput = strtok(NULL, delim);
    }
    line[i] = NULL;
	i = 0;


	
	while(line[i] != NULL){

		pid_t pid;			// process id variable
		int   status;		// integer used for status passed into waitpid()
	
		int j = 0;												// index variable
		char **cmd = malloc(512 * sizeof(char*));				// command array of strings allocated with 512 chars of space
		char  *space = " ";										// field seperater in input for parsing
		char  *parsedLine = strtok(line[i++], space);			// holds the parsed input line from the user
		
		// parsing Lines
		while(parsedLine != NULL)
		{
			cmd[j] = parsedLine;								// putting parsed input into the cmd
			j++;
		
			parsedLine = strtok(NULL, space);
		}
		cmd[j] = NULL;
	
		// added initial check for exit. Works only for single command input
		if(strcmp(cmd[0], "exit") == 0)
		{
			exit(0);
		}
		
		// executing process
		pid = fork();
		if(pid == 0)		// CHILD
		{
			// executing the given command and checking for failure
			if(execvp(cmd[0], cmd) < 0)
			{
					printf("%s command not found\n", cmd[0]);
					exit(1);		
			}
		}
		else if(pid == -1)	// ERROR
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		else				// PARENT
		{
			// waiting for the child to finish
			waitpid(pid, &status, WUNTRACED);
		}
		
		// freeing up memory
		free(cmd);
	}
}
