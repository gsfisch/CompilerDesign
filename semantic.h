#ifndef SEMANTIC_HEADER
#define SEMANTIC_HEADER

#include "ast.h"
#include "hash.h"

extern int semanticErrors;

typedef struct dataTypePair {
    int data_type_left;
    int data_type_right;
} DATA_TYPE_PAIR;

void check_and_set_declaration(AST* node);
void check_undeclared();
int get_semantic_errors();
void check_operands(AST* node);
int isArithmetic(AST* node);
int findExprDataType(AST *node);
void check_indexing(AST *node);
void check_functions_args_and_params(AST *node);
void set_function_datatype_params(AST *node);
int isParamAndArgsCompatible(int *params, int *args);
void checkAssignments(AST *node);
void checkFunctionReturn(AST *node);
void findReturnDataType(AST *node, int *answer);
void setArraySize(AST *node);
int interpretExpr(AST *node);
void checkArraySize(AST *node);
void checkArrayDeclarationTypes(AST* node);
void checkAllArrayTypes(AST* node, int data_type, int number_of_elements, int array_size);

#endif // SEMANTIC_HEADER
