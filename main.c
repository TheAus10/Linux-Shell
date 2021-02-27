/*
*
* GROUP 5
* MAJOR 2
*
*/


#include "processor.h"

#define BUFSIZE 512

int main(int argc, char *argv[]){
	char *myhistory[20];
	int historySize = 0;
	int nextHistory = 0;
	char *input;		// char pointer to store user input
	if((input = malloc(BUFSIZE*sizeof(char))) == NULL){
		perror("malloc");
		exit(1);
	}

	// signal handling
	struct sigaction act;						// signal struct
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

	if(argc > 2)		// error
	{
		printf("\nUsage: ./major2 <optional batch file>\n\n");
	}
	else if(argc == 2)	// batch mode
	{
		FILE*  batchFile = fopen(argv[1], "r");

		if(batchFile != NULL)		// file is opened correctly
		{
			// char pointer to store user input
			if((input = malloc(BUFSIZE*sizeof(char))) == NULL){
				perror("malloc");
				exit(1);
			}
			while(fgets(input, BUFSIZE, batchFile))
			{

				printf("From file: %s", input);

				if(nextHistory >= 20){
					nextHistory = 0;
				}

				char* temp;
				if((temp = malloc(BUFSIZE*sizeof(char))) == NULL){
					perror("malloc");
					exit(1);
				}
				strcpy(temp, input);


				myhistory[nextHistory] = temp;
				nextHistory++;
				historySize++;
				if(historySize > 20){
					historySize = 20;
				}

				processor(input, &historySize, myhistory, &nextHistory);
				printf("\n");
			}
		}
		else			// error
		{
			printf("Unable to open file\n");
			exit(-1);
		}

		// closing file
		fclose(batchFile);
	}
	else				// interactive mode
	{
		while(1)
		{
			// char pointer to store user input
			if((input = malloc(BUFSIZE*sizeof(char))) == NULL){
				perror("malloc");
				exit(1);
			}
			// prompts for and reads in users command
			printf("major2> ");
			if(fgets(input, BUFSIZE, stdin) == NULL){
				perror("fgets");
				exit(1);
			}
			int len = strlen(input);
			if(input[len-1] == '\n'){
				input[len-1] = '\0';
			}

			if(nextHistory >= 20){
				nextHistory = 0;
			}

			char* temp;
			if((temp = malloc(BUFSIZE*sizeof(char))) == NULL){
				perror("malloc");
				exit(1);
			}
			strcpy(temp, input);

			myhistory[nextHistory] = temp;
			nextHistory++;
			historySize++;
			if(historySize > 20){
				historySize = 20;
			}
			// passing input to processor function

			processor(input, &historySize, myhistory, &nextHistory);
		}
	}

	return 0;
}
