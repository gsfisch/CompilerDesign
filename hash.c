#include "hash.h"

HASH_NODE*Table[HASH_SIZE];

void hashInit(void)
{
    int i;

    for(i = 0; i < HASH_SIZE; i++)
        Table[i] = 0;
}

int hashAddress(char *text)
{
    int address = 1;
    int i;

    for(i = 0; i < strlen(text); i++)
        address = (address * text[i]) % HASH_SIZE + 1;

    return address - 1;
}

HASH_NODE *hashFind(char *text)
{
    HASH_NODE *node;

    int address = hashAddress(text);

    for(node = Table[address]; node; node = node->next)
        if (strcmp (node->text, text) == 0)
            return node;

    return 0;
}

 HASH_NODE *hashInsert(char *text, int type)
 {
    HASH_NODE *newnode;
    int address = hashAddress(text);

    if ( (newnode = hashFind(text)) != 0 )
            return newnode;

    newnode = (HASH_NODE*) calloc(1, sizeof(HASH_NODE));
    newnode->type = type;
    newnode->text = (char*) calloc(strlen(text)+1, sizeof(char));
    strcpy(newnode->text, text);
    newnode->next = Table[address];

    for(int i = 0; i < MAX_FUNCTION_PARAMS; i++)
        newnode->function_datatype_params[i] = 0;

    Table[address] = newnode;
    return newnode;
 }

 void hashPrint(void)
 {
     int i;
     HASH_NODE *node;

     for(i = 0; i < HASH_SIZE; i++)
        for(node = Table[i]; node; node = node->next)
        {
            printf("Table[%d] has %s with data_type ", i, node->text);

            switch(node->data_type)
            {
                case DATATYPE_CHAR: printf("CHAR"); break;
                case DATATYPE_INT: printf("INT"); break;
                case DATATYPE_FLOAT: printf("FLOAT"); break;
                case DATATYPE_BOOL: printf("BOOL"); break;
                default: printf("UNDEFINED"); break;
            }

            printf(" and type ");

            switch(node->type)
            {
                case SYMBOL_LIT_INTE: printf("LIT INT\n"); break;
                case SYMBOL_LIT_CARA: printf("LIT CHAR\n"); break;
                case SYMBOL_LIT_FLOAT: printf("LIT FLOAT\n"); break;
                case SYMBOL_LIT_STRING: printf("LIT STRING\n"); break;
                case SYMBOL_SCALAR: printf("SCALAR\n"); break;
                case SYMBOL_ARRAY: printf("ARRAY\n"); break;
                case SYMBOL_FUNC: printf("FUNCTION\n"); break;

                default: printf("UNDEFINED\n"); break;
            }
        }
 }

 int hash_check_undeclared()
 {
    int i, undeclared = 0;
     HASH_NODE *node;

     for(i = 0; i < HASH_SIZE; i++)
        for(node = Table[i]; node; node = node->next)
        {
            if (node->type == SYMBOL_IDENTIFIER)
            {
                printf("Semantic error: Variable %s not defined.\n", node->text);
                undeclared++;
            }
        }

    return undeclared;
 }

 int isCompatibleHash(HASH_NODE* node1, HASH_NODE* node2)
 {
     switch (node1->data_type)
     {
        case DATATYPE_CHAR:
        case DATATYPE_INT:
            if (node2->data_type == DATATYPE_INT || node2->data_type == DATATYPE_CHAR)
                return 1;
            return 0;

        case DATATYPE_FLOAT:
            if (node2->data_type == DATATYPE_FLOAT)
                return 1;
            return 0;

        case DATATYPE_BOOL:
            if (node2->data_type == DATATYPE_BOOL)
                return 1;
            return 0;

        default: return 0;
     }
 }

 int isCompatible(int dataType1, int dataType2)
 {
     switch (dataType1)
     {
        case DATATYPE_CHAR:
        case DATATYPE_INT:
            if (dataType2 == DATATYPE_INT || dataType2 == DATATYPE_CHAR)
                return 1;
            return 0;

        case DATATYPE_FLOAT:
            if (dataType2 == DATATYPE_FLOAT)
                return 1;
            return 0;

        case DATATYPE_BOOL:
            if (dataType2 == DATATYPE_BOOL)
                return 1;
            return 0;

        default: return 0;
     }
 }

HASH_NODE* makeTemp(void)
{
    static int serial = 0;
    char buffer[256] = "";

    sprintf(buffer, "mY___weirdTtttemp%d", serial++);
    return hashInsert(buffer, SYMBOL_SCALAR);
}