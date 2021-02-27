/*
 *
 * GROUP 5
 * MAJOR 2
 *
 * *** compile with -lreadline ***
 *
 */


#include "processor.h"

int main(int argc, char *argv[]){

	char *input;		// char pointer to store user input
	
	if(argc > 2)		// error
	{
		printf("\nUsage: ./major2 <optional batch file>\n\n");
	}
	else if(argc == 2)	// batch mode
	{
		FILE*  batchFile = fopen(argv[1], "r");
		
		if(batchFile != NULL)		// file is opened correctly
		{
			while(fgets(input, 512, batchFile))
			{
				printf("From file: %s", input);
				processor(input);	//FIXME
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
			// prompts for and reads in users command
			input = readline("major2> ");	
		
			// passing input to processor function
			processor(input);	
		}
	}
	
	return 0;
}
