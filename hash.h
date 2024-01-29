// C�digo obtido do professor dr. Marcelo Johann - UFRGS


#ifndef HASH_HEADER
#define HASH_HEADER

#define SYMBOL_LIT_INTE 1
#define SYMBOL_LIT_CARA 2
#define SYMBOL_LIT_FLOAT 3
#define SYMBOL_IDENTIFIER 4
#define SYMBOL_LIT_STRING 5
#define SYMBOL_SCALAR 6
#define SYMBOL_ARRAY 7
#define SYMBOL_FUNC 8
#define SYMBOL_IDENTIFIER_PARAM 9
#define SYMBOL_LABEL 10

#define DATATYPE_CHAR 1
#define DATATYPE_INT 2
#define DATATYPE_FLOAT 3
#define DATATYPE_BOOL 4

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 997
#define MAX_FUNCTION_PARAMS 20


struct value_node;

typedef struct hash_node
{
    int type;
    int data_type;
    char *text;
    int array_size;
    int function_datatype_params[MAX_FUNCTION_PARAMS];
    struct value_node *values;
    struct hash_node *next;
} HASH_NODE;

typedef struct value_node
{
    struct hash_node *value;
    struct value_node *next;
} VALUE_NODE;

void hashInit(void);
int hashAddress(char *text);
HASH_NODE *hashFind(char *text);
HASH_NODE *hashInsert(char *text, int type);
void hashPrint(void);
int hash_check_undeclared();
int isCompatibleHash(HASH_NODE* node1, HASH_NODE* node2);
int isCompatible(int dataType1, int dataType2);
HASH_NODE* makeTemp(void);

#endif

// END
