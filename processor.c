/*
*
* Group 5
* Major 2
*
*/

#include "processor.h"

void processor(char* input, int *historySize, char* myhistory[20], int *nextHistory){


	struct sigaction act;										// signal struct

	char *temp = malloc(8 * sizeof(char));						// temp string	(stores " exit ")
	int   exitCheck = 0;										// bool variable (set to 1 in exit code)

	// checking for exit and removing it from input
	while((temp = strstr(input, "exit")))
	{
		memmove(temp, temp +4, strlen(temp + 4) +1);
		exitCheck = 1;		// setting bool to true for check @ end of prog
	}

	char **line = malloc(512 * sizeof(char*)); 					// array of strings to store each command line
	char *delim = ";\n";										// field seperator
	char *parsedInput = strtok(input, delim); 					// holds input parsed at semi-colons
	int   i=0;													// index variable

	// parsing input by semi-colons
	while(parsedInput != NULL)
	{
		line[i] = parsedInput;                          		// putting parsed input into the cmd
		i++;

		parsedInput = strtok(NULL, delim);
	}
	line[i] = NULL;

	i = 0;
	while(line[i] != NULL){

		act.sa_handler = SIG_IGN;

		for(int n = 1; n <= 64; n++)
		{
			// catching all catch-able signals
			if(n ==9)
				continue;
			else if(n == 19)
				continue;
			else if(n == 32)
				continue;
			else if(n == 33)
				continue;

			// setting how to handle recieved signals
			assert(sigaction(n, &act, NULL) == 0);	// ERROR CHECK
		}

		pid_t pid;												// process id variable
		int   status;											// integer used for status passed into waitpid()

		int j = 0;												// index variable

		char **cmd = malloc(512 * sizeof(char*));				// command array of strings allocated with 64 chars of space
		char  *space = " ";										// field seperater in input for parsing
		char  *parsedLine = strtok(line[i++], space);			// holds the parsed input line from the user

		// parsing lines by spaces
		while(parsedLine != NULL)
		{
			cmd[j] = parsedLine;								// putting parsed input into the cmd
			j++;

			parsedLine = strtok(NULL, space);
		}
		cmd[j] = NULL;

		// ignoring any commands set to NULL
		if(!cmd[0])
		{
			continue;
		}

		//new "cd" command with argument
		if(strcmp(cmd[0], "cd") == 0)
		{
			if(!cmd[1])
			{
				//system call just to check that chdir() worked, erase after testing.
				chdir("/home");
				system("pwd");
			}

			if(cmd[1])
			{
				chdir(cmd[1]);
				system("pwd");
			}
			break;
		}

		// new "myhistory" user command
		if(strcmp(cmd[0], "myhistory") == 0){
			if(!cmd[1]){
				printf("Command History:\n");
				for(int k = 0; k < *historySize; k++){
					printf("[%d] %s\n", k+1, myhistory[k]);
				}

			}
			else if(strcmp(cmd[1], "-e") == 0){
				if(!cmd[2]){
					printf("ERROR: Usage: myhistory -e <history number>\n");
					break;
				}
				else{
					int history = *cmd[2] - '0';
					char *temp;
					if((temp = malloc(512*sizeof(char))) == NULL){
						perror("malloc");
						exit(1);
					}
					strcpy(temp, myhistory[history-1]);
					processor(temp, historySize, myhistory, nextHistory);
					break;
				}
			}
			else if(strcmp(cmd[1], "-c") == 0){
				for(int k = 0; k < *historySize; k++){
					free(myhistory[k]);
				}
				*historySize = 0;
				*nextHistory = 0;
			}


			break;
		}


		// new "path" user command
		if(strcmp(cmd[0], "path") == 0){

			// print pathname, if cmd[1] is NULL
			if(!cmd[1]){
				printf("%s\n", getenv("PATH"));
			}
			// remove pathname
			else if(strcmp(cmd[1], "-") == 0 && cmd[2]){
				char *tempPath = getenv("PATH");
				char *pos = strstr(tempPath, cmd[2]);
				memmove(pos-1, pos+strlen(cmd[2]), strlen(cmd[2])+1);
				setenv("PATH", tempPath, 1);
			}
			// append pathname in cmd[2]
			else if(strcmp(cmd[1], "+") == 0 && cmd[2]){
				char *tempPath = getenv("PATH");
				strcat(tempPath, ":");
				strcat(tempPath, cmd[2]);
				setenv("PATH", tempPath, 1);
			}
			else{
				perror("path");
			}
			break;
		}

		// executing process
		pid = fork();
		if(pid == 0)		// CHILD
		{
			printf("CHILD PID: %d\n", getpid());
			printf("PARENT PID: %d\n", getppid());

			// signal handling
			act.sa_handler = SIG_DFL;			// performing default action when signal recieved
			for(int n = 1; n <= 64; n++)
			{
				// catching all catch-able signals
				if(n ==9)
					continue;
				else if(n == 19)
					continue;
				else if(n == 32)
					continue;
				else if(n == 33)
					continue;

				// setting how to handle recieved signals
				assert(sigaction(n, &act, NULL) == 0);	// ERROR CHECK
			}

			/*Checking for a '<' & '>' symbol and storing strings with commands/file names.*/
			int redirect = 0; //bool statement
			int piper = 0;
			for(int p=0; p < j; p++)
			{
				if(strchr(cmd[p], '<')!=NULL)
				{
					int ifp;
					redirect = 1;
					char inputF[256];
					char *redirectionCMD [j];

					for(int k=0; k <= j; k++)
					{
						char *strResult = strchr(cmd[k], '<');

						if(strResult == NULL)
						{
							redirectionCMD[k]=cmd[k];
						}
						if(strResult != NULL) //found redirection signal.
						{
							redirectionCMD[k]='\0';
							strcpy(inputF, cmd[++k]);       //input file is 1 space after redirection signal.
							redirectionCMD[k]='\0';
							break;

						}

					}
					ifp = open(inputF, O_RDONLY);
					dup2(ifp,fileno(stdin));
					close(ifp);
					execvp(redirectionCMD[0], redirectionCMD);
					dup2(0,fileno(stdin));
					printf("%s: command not found\n", redirectionCMD[0]);
					break;
				}
				if(strchr(cmd[p], '>')!=NULL)
				{
					redirect = 1;
					int ofp;

					char inputF[256];
					char *redirectionCMD [j];
					int k = 0;
					for(k=0; k < j; k++)
					{
						char *strResult = strchr(cmd[k], '>');

						if(strResult == NULL)
						{
							redirectionCMD[k]=cmd[k];
						}
						if(strResult != NULL) //found redirection signal.
						{
							redirectionCMD[k]='\0';
							strcpy(inputF, cmd[++k]);       //input file is 1 space after redirection signal.
							redirectionCMD[k]='\0';
							break;
						}
					}
					ofp = open(inputF, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
					dup2(ofp,fileno(stdout));
					close(ofp);
					execvp(redirectionCMD[0], redirectionCMD);
					dup2(1, fileno(stdout));
					printf("%s: command not found\n", redirectionCMD[0]);
					break;
				}


				// handling commands that use pipelining
				if(strchr(cmd[p], '|') !=NULL){
					piper = 1;
					int twopipes = 0;
					char *pipe1CMD[j];
					char *pipe1outCMD[j];
					char *pipe2outCMD[j];
					int firsttime = 1;

					int k = 0;
					int out1ind = 0;
					int out2ind = 0;
					while(cmd[k] != NULL)
					{
						char *strResult = strchr(cmd[k], '|');

						if((strResult == NULL) && (firsttime == 1))
						{
							pipe1CMD[k]=cmd[k];
						}
						else if((strResult != NULL) && (firsttime == 1)) //found 1st pipe signal.
						{
							firsttime = 0;
							pipe1CMD[k]='\0';
						}
						else if((strResult == NULL) && (firsttime == 0) && (twopipes == 0))
						{
							pipe1outCMD[out1ind]=cmd[k];
							out1ind++;
						}
						else if((strResult != NULL) && (firsttime == 0))
						{
							twopipes = 1;
						}
						else if(((strResult == NULL) && (firsttime == 0)) && (twopipes == 1))
						{
							pipe2outCMD[out2ind]=cmd[k];
							out2ind++;
						}
						k++;
					}

					pipe1outCMD[out1ind] = '\0';

					if(twopipes == 0){
						int fd1[2];
						pipe(fd1);
						if(fork() == 0){
							dup2(fd1[1], fileno(stdout));

							close(fd1[1]);
							close(fd1[0]);

							execvp(pipe1CMD[0], pipe1CMD);
						}
						else
						{
							if(fork() == 0){
								dup2(fd1[0], fileno(stdin));

								close(fd1[0]);
								close(fd1[1]);

								execvp(pipe1outCMD[0], pipe1outCMD);
							}
						}

						close(fd1[0]);
						close(fd1[1]);

						for(int waiter = 0; waiter < 2; waiter++){
							wait(&status);
						}
					}
					else if(twopipes == 1){
						int fd1[2], fd2[2];
						pipe(fd1);
						pipe(fd2);
						if(fork() == 0){
							dup2(fd1[1], fileno(stdout));

							close(fd1[1]);
							close(fd1[0]);
							close(fd2[1]);
							close(fd2[0]);

							execvp(pipe1CMD[0], pipe1CMD);
						}
						else
						{
							if(fork() == 0){
								dup2(fd1[0], fileno(stdin));

								dup2(fd2[1], fileno(stdout));

								close(fd1[0]);
								close(fd1[1]);
								close(fd2[0]);
								close(fd2[1]);

								execvp(pipe1outCMD[0], pipe1outCMD);
							}
							else{
								dup2(fd2[0], fileno(stdin));

								close(fd1[0]);
								close(fd1[1]);
								close(fd2[0]);
								close(fd2[1]);

								execvp(pipe2outCMD[0], pipe2outCMD);
							}

							close(fd1[0]);
							close(fd1[1]);
							close(fd2[0]);
							close(fd2[1]);

							for(int waiter = 0; waiter < 3; waiter++){
								wait(&status);
							}
						}
					}
				}
			}
			if(redirect==1)
			{
				redirect = 0;
				abort();
				break;
			}
			if(piper==1)
			{
				piper = 0;
				abort();
				break;
			}

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
			signal(SIGTTOU, SIG_IGN);

			// waiting for the child to finish
			waitpid(pid, &status, WUNTRACED);
		}

		// freeing up memory
		free(cmd);
	}

	// exits if the line has an exit in it
	if(exitCheck == 1)
	{
		free(temp);
		exit(0);
	}
}
