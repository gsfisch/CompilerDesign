#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "semantic.h"


extern FILE *yyin;
extern int lineNumber;

FILE *outputFile;
int yyparse();

int main(int argc, char **argv)
{
	int tok;

	if (argc != 3)
	{
		fprintf(stderr, "Call: /etapa3 input_file_name output_file_name\n");
		exit(1);
	}

	if (!(yyin = fopen(argv[1], "r")))
	{
		fprintf(stderr, "File could not be opened\n");
		exit(2);
	}

	if (!(outputFile = fopen(argv[2], "w")))
	{
		fprintf(stderr, "File could not be opened\n");
		exit(2);
	}

	yyparse();

	printf("Program has %d line(s).\n", lineNumber - 1);

	hashPrint();

	if( get_semantic_errors() )
    {
        fprintf(stderr, "Number of errors: %d\n", get_semantic_errors());
        exit(4);
    }

	exit(0);
}
