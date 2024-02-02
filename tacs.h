#ifndef TACS_HEADER
#define TACS_HEADER

#include "ast.h"
#include "hash.h"

#define TAC_SYMBOL 1
#define TAC_ADD 2
#define TAC_SUB 3
#define TAC_MULTIPLY 4
#define TAC_DIVIDE 5
#define TAC_ASG 6
#define TAC_ASG_ARRAY 7
#define TAC_JFALSE 8
#define TAC_LABEL 9
#define TAC_ARRAY_ACCESS 10
#define TAC_BEGINFUN 11
#define TAC_ENDFUN 12
#define TAC_JUMP 13
#define TAC_CALL 14
#define TAC_ARG 15
#define TAC_RET 16
#define TAC_PRINT 17
#define TAC_READ 18
#define TAC_LT 19
#define TAC_GT 20
#define TAC_LE 21
#define TAC_GE 22
#define TAC_EQ 23
#define TAC_DIF 24
#define TAC_AND 25
#define TAC_OR 26
#define TAC_NOT 27


typedef struct tac
{
    int type;
    HASH_NODE* result;
    HASH_NODE* operand1;
    HASH_NODE* operand2;
    struct tac* prev;
    struct tac* next;
} TAC;


TAC* tacCreate(int type, HASH_NODE *result, HASH_NODE *operand1, HASH_NODE *operand2);
void tacPrint(TAC* node);
void tacPrintBackwards(TAC* node);
TAC* tacGenerateCode(AST *node);
TAC* tacJoin(TAC* tacList1, TAC* tacList2);



#endif