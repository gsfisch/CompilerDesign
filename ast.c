#include <stdio.h>
#include <stdlib.h>
#include "ast.h"


AST *astCreate(int type, HASH_NODE *symbol, AST* s0, AST* s1, AST* s2, AST* s3)
{
	AST *newnode = (AST*) malloc(sizeof(AST));

	newnode->type = type;
	newnode->symbol = symbol;
	newnode->son[0] = s0;
	newnode->son[1] = s1;
	newnode->son[2] = s2;
	newnode->son[3] = s3;

	return newnode;
}

void astPrint(AST *node, int level)
{
	if (!node)
		return;

	int i = 0;

	for(int j = 0; j < level; j++)
		fprintf(stderr, "  ");

	fprintf(stderr, "ast(");

	switch(node->type)
	{
		case AST_SYMBOL: fprintf(stderr, "AST_SYMBOL"); break;
		case AST_ADD: fprintf(stderr, "AST_ADD"); break;
		case AST_SUB: fprintf(stderr, "AST_SUB"); break;
		case AST_MULTIPLY: fprintf(stderr, "AST_MULTIPLY"); break;
		case AST_DIVIDE: fprintf(stderr, "AST_DIVIDE"); break;
		case AST_LT: fprintf(stderr, "AST_LT"); break;
		case AST_GT: fprintf(stderr, "AST_GT"); break;
		case AST_LE: fprintf(stderr, "AST_LE"); break;
		case AST_GE: fprintf(stderr, "AST_GE"); break;
		case AST_EQ: fprintf(stderr, "AST_EQ"); break;
		case AST_DIF: fprintf(stderr, "AST_DIF"); break;
		case AST_AND: fprintf(stderr, "AST_AND"); break;
		case AST_OR: fprintf(stderr, "AST_OR"); break;
		case AST_NOT: fprintf(stderr, "AST_NOT"); break;
		default: fprintf(stderr, "AST_UNKNOWN"); break;
	}

	if (node->symbol)
		fprintf(stderr, ", %s\n", node->symbol->text);
	else
		fprintf(stderr, ", 0\n");

	for(i = 0; i < MAX_SONS; i++)
		astPrint(node->son[i], level + 1);

	for(int j = 0; j < level; j++)
		fprintf(stderr, "  ");
	fprintf(stderr, ")\n");
}


void astDecompilation(AST *node, FILE* outputFile)
{
    if (!node)
        return;

    switch (node->type)
    {
        // program
        case AST_ROOT: astDecompilation(node->son[0], outputFile); astDecompilation(node->son[1], outputFile); return; break;


        // globalDeclarations
        case AST_SCALAR_CHAR_DEC: fprintf(outputFile, " char %s=", node->symbol->text); astDecompilation(node->son[0], outputFile); fprintf(outputFile, ";"); astDecompilation(node->son[1], outputFile);   return;
        case AST_SCALAR_INT_DEC: fprintf(outputFile, " int %s=", node->symbol->text); astDecompilation(node->son[0], outputFile); fprintf(outputFile, ";"); astDecompilation(node->son[1], outputFile);    return;
        case AST_SCALAR_FLOAT_DEC: fprintf(outputFile, " float %s=", node->symbol->text); astDecompilation(node->son[0], outputFile); fprintf(outputFile, ";"); astDecompilation(node->son[1], outputFile);   return;
        case AST_ARRAY_DEC: astDecompilation(node->son[0], outputFile); astDecompilation(node->son[1], outputFile); return;
        case AST_FUN_DEC: astDecompilation(node->son[0], outputFile); astDecompilation(node->son[1], outputFile); return;


        // functionDeclaration, parameterList, parameterListTail and parameterListNotEmpty
        case AST_FUN_CHAR: fprintf(outputFile, " char %s(", node->symbol->text); astDecompilation(node->son[0], outputFile); fprintf(outputFile, ");"); return;
        case AST_FUN_INT: fprintf(outputFile, " int %s(", node->symbol->text); astDecompilation(node->son[0], outputFile); fprintf(outputFile, ");"); return;
        case AST_FUN_FLOAT: fprintf(outputFile, " float %s(", node->symbol->text); astDecompilation(node->son[0], outputFile); fprintf(outputFile, ");"); return;
        case AST_PARAM_ITEM: astDecompilation(node->son[0], outputFile); fprintf(outputFile, "%s", node->symbol->text); astDecompilation(node->son[1], outputFile); return;
        case AST_PARAM_ITEM_ARRAY: astDecompilation(node->son[0], outputFile); fprintf(outputFile, "%s []", node->symbol->text); astDecompilation(node->son[1], outputFile); return;
        case AST_PARAM_ITEM_NEXT: fprintf(outputFile, ","); astDecompilation(node->son[0], outputFile); return;
        case AST_PARAM_TYPE_CHAR: fprintf(outputFile, " char "); return;
        case AST_PARAM_TYPE_INT: fprintf(outputFile, " int "); return;
        case AST_PARAM_TYPE_FLOAT: fprintf(outputFile, " float "); return;


        // arrayDeclaration and arrayInit
        case AST_ARRAY_CHAR_INIT: fprintf(outputFile, " char %s[", node->symbol->text); astDecompilation(node->son[0], outputFile); fprintf(outputFile, "]"); astDecompilation(node->son[1], outputFile); fprintf(outputFile, ";"); return;
        case AST_ARRAY_CHAR: fprintf(outputFile, " char %s[", node->symbol->text); astDecompilation(node->son[0], outputFile); fprintf(outputFile, "]"); fprintf(outputFile, ";"); return;
        case AST_ARRAY_INT_INIT: fprintf(outputFile, " int %s[", node->symbol->text); astDecompilation(node->son[0], outputFile); fprintf(outputFile, "]"); astDecompilation(node->son[1], outputFile); fprintf(outputFile, ";"); return;
        case AST_ARRAY_INT: fprintf(outputFile, " int %s[", node->symbol->text); astDecompilation(node->son[0], outputFile); fprintf(outputFile, "]"); fprintf(outputFile, ";"); return;
        case AST_ARRAY_FLOAT_INIT: fprintf(outputFile, " float %s[", node->symbol->text); astDecompilation(node->son[0], outputFile); fprintf(outputFile, "]"); astDecompilation(node->son[1], outputFile); fprintf(outputFile, ";"); return;
        case AST_ARRAY_FLOAT: fprintf(outputFile, " float %s[", node->symbol->text); astDecompilation(node->son[0], outputFile); fprintf(outputFile, "]"); fprintf(outputFile, ";"); return;
        case AST_ARRAY_INIT_ITEM: fprintf(outputFile, " "); astDecompilation(node->son[0], outputFile); astDecompilation(node->son[1], outputFile); return;


        // codeList and funcDefinition
        case AST_FUN_DEF: astDecompilation(node->son[0], outputFile); astDecompilation(node->son[1], outputFile); return; break;
        case AST_CODE: fprintf(outputFile, "code %s", node->symbol->text); astDecompilation(node->son[0], outputFile); return; break;


        // command
        case AST_ASG: fprintf(outputFile, "%s =", node->symbol->text); astDecompilation(node->son[0], outputFile); fprintf(outputFile, ";"); return; break;
		case AST_ASG_ARRAY: fprintf(outputFile, "%s[", node->symbol->text); astDecompilation(node->son[0], outputFile); fprintf(outputFile, "]="); astDecompilation(node->son[1], outputFile); fprintf(outputFile, ";"); return; break;
        case AST_PRINT_LIT: fprintf(outputFile, " print "); astDecompilation(node->son[0], outputFile); fprintf(outputFile, ";"); return; break;
		case AST_PRINT_EXPR: fprintf(outputFile, " print "); astDecompilation(node->son[0], outputFile); fprintf(outputFile, ";"); return; break;
		case AST_RETURN: fprintf(outputFile, " return "); astDecompilation(node->son[0], outputFile); fprintf(outputFile, ";"); return; break;
        case AST_STRING: fprintf(outputFile, " %s ", node->symbol->text); return;

        // controlStatement
        case AST_IF: fprintf(outputFile, " if ("); astDecompilation(node->son[0], outputFile); fprintf(outputFile, ")"); astDecompilation(node->son[1], outputFile); return; break;
        case AST_IFELSE: fprintf(outputFile, " if ("); astDecompilation(node->son[0], outputFile); fprintf(outputFile, ")"); astDecompilation(node->son[1], outputFile); fprintf(outputFile, "else"); astDecompilation(node->son[2], outputFile); return; break;
        case AST_WHILE: fprintf(outputFile, " while ("); astDecompilation(node->son[0], outputFile); fprintf(outputFile, ")"); astDecompilation(node->son[1], outputFile); return; break;


        // commandBlock and commandList
        case AST_CMDB: fprintf(outputFile, "{"); astDecompilation(node->son[0], outputFile); fprintf(outputFile, "}"); return; break;
        case AST_CMDL: astDecompilation(node->son[0], outputFile); astDecompilation(node->son[1], outputFile); return; break;


        // expr
        case AST_SYMBOL: fprintf(outputFile, "%s", node->symbol->text); break;
		case AST_ADD: astDecompilation(node->son[0], outputFile); fprintf(outputFile, "+"); astDecompilation(node->son[1], outputFile); return; break;
		case AST_SUB: astDecompilation(node->son[0], outputFile); fprintf(outputFile, "-"); astDecompilation(node->son[1], outputFile); return; break;
		case AST_MULTIPLY: astDecompilation(node->son[0], outputFile); fprintf(outputFile, "*"); astDecompilation(node->son[1], outputFile); return; break;
		case AST_DIVIDE: astDecompilation(node->son[0], outputFile); fprintf(outputFile, "/"); astDecompilation(node->son[1], outputFile); return; break;
		case AST_LT: astDecompilation(node->son[0], outputFile); fprintf(outputFile, "<"); astDecompilation(node->son[1], outputFile); return; break;
		case AST_GT: astDecompilation(node->son[0], outputFile); fprintf(outputFile, ">"); astDecompilation(node->son[1], outputFile); return; break;
		case AST_LE: astDecompilation(node->son[0], outputFile); fprintf(outputFile, "<="); astDecompilation(node->son[1], outputFile); return; break;
		case AST_GE: astDecompilation(node->son[0], outputFile); fprintf(outputFile, ">="); astDecompilation(node->son[1], outputFile); return; break;
		case AST_EQ: astDecompilation(node->son[0], outputFile); fprintf(outputFile, "=="); astDecompilation(node->son[1], outputFile); return; break;
		case AST_DIF: astDecompilation(node->son[0], outputFile); fprintf(outputFile, "!="); astDecompilation(node->son[1], outputFile); return; break;
		case AST_AND: astDecompilation(node->son[0], outputFile); fprintf(outputFile, "&&"); astDecompilation(node->son[1], outputFile); return; break;
		case AST_OR: astDecompilation(node->son[0], outputFile); fprintf(outputFile, "||"); astDecompilation(node->son[1], outputFile); return; break;
		case AST_NOT: fprintf(outputFile, "!"); astDecompilation(node->son[0], outputFile); return; break;
		case AST_INPUT: fprintf(outputFile, " input ("); astDecompilation(node->son[0], outputFile); fprintf(outputFile, ")"); return; break;
		case AST_FUN_CALL: fprintf(outputFile, "%s(", node->symbol->text); astDecompilation(node->son[0], outputFile); fprintf(outputFile, ")"); return; break;
        case AST_ARRAY_ACCESS: astDecompilation(node->son[0], outputFile); fprintf(outputFile, "["); astDecompilation(node->son[1], outputFile); fprintf(outputFile, "]"); return; break;


        // inputArgument
        case AST_INPUT_ARG_CHAR: fprintf(outputFile, " char "); return; break;
        case AST_INPUT_ARG_INT: fprintf(outputFile, " int "); return; break;
        case AST_INPUT_ARG_FLOAT: fprintf(outputFile, " float "); return; break;


        // exprList, exprListTail and exprNotEmpty
        case AST_EXPR_LIST_ITEM: astDecompilation(node->son[0], outputFile); astDecompilation(node->son[1], outputFile); return; break;
        case AST_EXPR_LIST_ITEM_NEXT: fprintf(outputFile, ","); astDecompilation(node->son[0], outputFile); astDecompilation(node->son[1], outputFile); return; break;


        default: fprintf(outputFile, " "); break;
    }

    for(int i = 0; i < 4; i++)
        astDecompilation(node->son[i], outputFile);
}
