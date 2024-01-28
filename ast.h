#include "hash.h"

#ifndef AST_HEADER
#define AST_HEADER


#define MAX_SONS 4

#define AST_SYMBOL 1	// CHECK
#define AST_ADD 2	// CHECK
#define AST_SUB 3	// CHECK
#define AST_MULTIPLY 4	// CHECK
#define AST_DIVIDE 5	// CHECK
#define AST_LT 6	// CHECK
#define AST_GT 7	// CHECK
#define AST_LE 8	// CHECK
#define AST_GE 9	// CHECK
#define AST_EQ 10	// CHECK
#define AST_DIF 11	// CHECK
#define AST_AND 12	// CHECK
#define AST_OR 13	// CHECK
#define AST_NOT 14	// CHECK
#define AST_CMDL 15

#define AST_IF 16	// CHECK
#define AST_IFELSE 17	// CHECK
#define AST_WHILE 18	// CHECK
#define AST_CMDB 19

#define AST_PRINT_LIT 20	// CHECK
#define AST_PRINT_EXPR 21	// CHECK
#define AST_RETURN 22	// CHECK
#define AST_ASG 23	// CHECK
#define AST_ASG_ARRAY 24	// CHECK
#define AST_FUN_DEF 25
#define AST_CODE 26
#define AST_INPUT 27
#define AST_FUN_CALL 28
#define AST_INPUT_ARG_CHAR 29
#define AST_EXPR_LIST_ITEM 30

#define AST_ROOT 31	// CHECK
#define AST_SCALAR_CHAR_DEC 32	// CHECK
#define AST_SCALAR_INT_DEC 33
#define AST_SCALAR_FLOAT_DEC 34
#define AST_ARRAY_DEC 35
#define AST_FUN_DEC 36
#define AST_ARRAY_INIT_ITEM 37
#define AST_FUN_CHAR 38
#define AST_FUN_INT 39
#define AST_FUN_FLOAT 40
#define AST_PARAM_ITEM 41

#define AST_ARRAY_CHAR_INIT 42
#define AST_ARRAY_CHAR 43

#define AST_ARRAY_INT_INIT 44
#define AST_ARRAY_INT 45

#define AST_ARRAY_FLOAT_INIT 46
#define AST_ARRAY_FLOAT 47

#define FUN_CALL 48

#define AST_ARRAY_ACCESS 49

#define AST_INPUT_ARG_INT 50
#define AST_INPUT_ARG_FLOAT 51

#define AST_EXPR_LIST_ITEM_NEXT 52
#define AST_PARAM_ITEM_NEXT 53
#define AST_PARAM_TYPE_CHAR 54
#define AST_PARAM_TYPE_INT 55
#define AST_PARAM_TYPE_FLOAT 56

#define AST_STRING 57

#define AST_PARAM_ITEM_ARRAY 58

typedef struct astnode
{
	int type;
	HASH_NODE *symbol;
	struct astnode *son[MAX_SONS];
} AST;


AST* astCreate(int type, HASH_NODE *symbol, AST* s0, AST* s1, AST* s2, AST* s3);
void astPrint(AST *node, int level);
void astDecompilation(AST *node, FILE* outputFile);


#endif
