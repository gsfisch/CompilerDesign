#include "semantic.h"


int semanticErrors = 0;

void check_and_set_declaration(AST* node)
{
    int i;

    if (!node)
        return;

    switch(node->type)
    {
        case AST_SYMBOL:
            if (node->symbol)
            {
                switch(node->symbol->type)
                {
                    case SYMBOL_LIT_INTE: node->symbol->data_type = DATATYPE_INT; break;
                    case SYMBOL_LIT_CARA: node->symbol->data_type = DATATYPE_CHAR; break;
                    case SYMBOL_LIT_FLOAT: node->symbol->data_type = DATATYPE_FLOAT; break;
                    default: break;
                }
            }

            break;

        case AST_SCALAR_CHAR_DEC:
            if (node->symbol)
            {
                if (node->symbol->type != SYMBOL_IDENTIFIER)
                {
                    fprintf(stderr, "Semantic Error: scalar %s redeclared\n", node->symbol->text);
                    semanticErrors++;
                }

                node->symbol->type = SYMBOL_SCALAR;
                node->symbol->data_type = DATATYPE_CHAR;
            }

            break;

        case AST_SCALAR_INT_DEC:
            if (node->symbol)
            {
                if (node->symbol->type != SYMBOL_IDENTIFIER)
                {
                    fprintf(stderr, "Semantic Error: scalar %s redeclared\n", node->symbol->text);
                    semanticErrors++;
                }

                node->symbol->type = SYMBOL_SCALAR;
                node->symbol->data_type = DATATYPE_INT;
            }

            break;

        case AST_SCALAR_FLOAT_DEC:
            if (node->symbol)
            {
                if (node->symbol->type != SYMBOL_IDENTIFIER)
                {
                    fprintf(stderr, "Semantic Error: scalar %s redeclared\n", node->symbol->text);
                    semanticErrors++;
                }

                node->symbol->type = SYMBOL_SCALAR;
                node->symbol->data_type = DATATYPE_FLOAT;
            }

            break;

        case AST_ARRAY_DEC:
            if (node->son[0]->symbol)
            {
                if (node->son[0]->symbol->type != SYMBOL_IDENTIFIER)
                {
                    fprintf(stderr, "Semantic Error: array %s redeclared\n", node->son[0]->symbol->text);
                    semanticErrors++;
                }

                node->son[0]->symbol->type = SYMBOL_ARRAY;

                switch(node->son[0]->type)
                {
                    case AST_ARRAY_CHAR_INIT:
                    case AST_ARRAY_CHAR: node->son[0]->symbol->data_type = DATATYPE_CHAR; break;
                    case AST_ARRAY_INT_INIT:
                    case AST_ARRAY_INT: node->son[0]->symbol->data_type = DATATYPE_INT; break;
                    case AST_ARRAY_FLOAT_INIT:
                    case AST_ARRAY_FLOAT: node->son[0]->symbol->data_type = DATATYPE_FLOAT; break;
                }
            }

            break;

        case AST_FUN_CHAR:
            if (node->symbol)
            {
                if (node->symbol->type != SYMBOL_IDENTIFIER)
                {
                    fprintf(stderr, "Semantic Error: function %s redeclared\n", node->symbol->text);
                    semanticErrors++;
                }

                node->symbol->type = SYMBOL_FUNC;
                node->symbol->data_type = DATATYPE_CHAR;
            }

            break;

        case AST_FUN_INT:
            if (node->symbol)
            {
                if (node->symbol->type != SYMBOL_IDENTIFIER)
                {
                    fprintf(stderr, "Semantic Error: function %s redeclared\n", node->symbol->text);
                    semanticErrors++;
                }

                node->symbol->type = SYMBOL_FUNC;
                node->symbol->data_type = DATATYPE_INT;
            }

            break;

        case AST_FUN_FLOAT:
            if (node->symbol)
            {
                if (node->symbol->type != SYMBOL_IDENTIFIER)
                {
                    fprintf(stderr, "Semantic Error: function %s redeclared\n", node->symbol->text);
                    semanticErrors++;
                }

                node->symbol->type = SYMBOL_FUNC;
                node->symbol->data_type = DATATYPE_FLOAT;
            }

            break;

        case AST_PARAM_ITEM:
            if (node->symbol)
            {
                if (node->symbol->type != SYMBOL_IDENTIFIER)
                {
                    fprintf(stderr, "Semantic Error: function parameter %s redeclared\n", node->symbol->text);
                    semanticErrors++;
                }

                node->symbol->type = SYMBOL_SCALAR;

                switch(node->son[0]->type)
                {
                    case AST_PARAM_TYPE_CHAR: node->symbol->data_type = DATATYPE_CHAR; break;
                    case AST_PARAM_TYPE_INT: node->symbol->data_type = DATATYPE_INT; break;
                    case AST_PARAM_TYPE_FLOAT: node->symbol->data_type = DATATYPE_FLOAT; break;
                }
            }

            break;

        case AST_PARAM_ITEM_ARRAY:
            if (node->symbol)
            {
                if (node->symbol->type != SYMBOL_IDENTIFIER)
                {
                    fprintf(stderr, "Semantic Error: function parameter %s redeclared\n", node->symbol->text);
                    semanticErrors++;
                }

                node->symbol->type = SYMBOL_ARRAY;

                switch(node->son[0]->type)
                {
                    case AST_PARAM_TYPE_CHAR: node->symbol->data_type = DATATYPE_CHAR; break;
                    case AST_PARAM_TYPE_INT: node->symbol->data_type = DATATYPE_INT; break;
                    case AST_PARAM_TYPE_FLOAT: node->symbol->data_type = DATATYPE_FLOAT; break;
                }
            }

            break;
    }

    for(i = 0; i < MAX_SONS; i++)
        check_and_set_declaration(node->son[i]);
}

void check_undeclared()
{
    semanticErrors += hash_check_undeclared();
}

int get_semantic_errors()
{
    return semanticErrors;
}

void check_operands(AST* node)
{
    int i;

    if (!node)
        return;

    switch(node->type)
    {
        case AST_ADD:
        case AST_SUB:
        case AST_MULTIPLY:
        case AST_DIVIDE:
            if  (!isArithmetic(node->son[0]))
            {
                fprintf(stderr, "Semantic Error: invalid left operand for ADD\n");
                semanticErrors++;
            }
            if  (!isArithmetic(node->son[1]))
            {
                fprintf(stderr, "Semantic Error: invalid left operand for ADD\n");
                semanticErrors++;
            }

            if (!findExprDataType(node))
            {
                fprintf(stderr, "Semantic Error: incompatible operands for ADD\n");
                semanticErrors++;
            }
    }

    for(i = 0; i < MAX_SONS; i++)
        check_operands(node->son[i]);
}

int isArithmetic(AST* node)
{
    if (
        node->type == AST_ADD ||
        node->type == AST_SUB ||
        node->type == AST_MULTIPLY ||
        node->type == AST_DIVIDE ||
        (node->type == AST_ARRAY_ACCESS &&
         (node->son[0]->symbol->data_type == DATATYPE_CHAR || node->son[0]->symbol->data_type == DATATYPE_INT || node->son[0]->symbol->data_type == DATATYPE_FLOAT)) ||
        (node->type == AST_SYMBOL &&
         (node->symbol->type == SYMBOL_LIT_INTE || node->symbol->type == SYMBOL_LIT_CARA || node->symbol->type == SYMBOL_LIT_FLOAT || node->symbol->type == SYMBOL_SCALAR)) ||
        (node->type == AST_FUN_CALL &&
         (node->symbol->data_type == DATATYPE_CHAR || node->symbol->data_type == DATATYPE_INT || node->symbol->data_type == DATATYPE_FLOAT))
        )
            return 1;

    return 0;
}

int findExprDataType(AST *node)
{
    int left_data_type;
    int right_data_type;

    if (!node)
        return 0;

    switch(node->type)
    {
        case AST_SYMBOL: return node->symbol->data_type;
        case AST_ARRAY_ACCESS: return node->son[0]->symbol->data_type;
        case AST_FUN_CALL: return node->symbol->data_type;


        case AST_INPUT:
            switch(node->son[0]->type)
            {
                case AST_INPUT_ARG_CHAR: return DATATYPE_CHAR;
                case AST_INPUT_ARG_INT: return DATATYPE_INT;
                case AST_INPUT_ARG_FLOAT: return DATATYPE_FLOAT;
            }


        case AST_LT:
        case AST_GT:
        case AST_LE:
        case AST_GE:
        case AST_EQ:
        case AST_DIF:
        case AST_AND:
        case AST_OR:
        case AST_NOT: return DATATYPE_BOOL;


        case AST_ADD:
        case AST_SUB:
        case AST_MULTIPLY:
        case AST_DIVIDE:
            left_data_type = findExprDataType(node->son[0]);
            right_data_type = findExprDataType(node->son[1]);

            //fprintf(stderr, "left: %d\nright: %d\n", left_data_type, right_data_type);

            if( !isCompatible(left_data_type, right_data_type) )
                return 0;

            // MUDAR AQUI ?
            return left_data_type;

        default: return 0;
    }
}

void check_indexing(AST *node)
{
    int i;

    if (!node)
        return;

    switch(node->type)
    {
        case AST_ARRAY_DEC:
            if( findExprDataType(node->son[0]->son[0]) != DATATYPE_CHAR && findExprDataType(node->son[0]->son[0]) != DATATYPE_INT )
            {
                fprintf(stderr, "Semantic Error: invalid array declaration index data type\n");
                semanticErrors++;
            }

            break;

        case AST_ASG_ARRAY:
            if ( findExprDataType(node->son[0]) != DATATYPE_CHAR && findExprDataType(node->son[0]) != DATATYPE_INT )
            {
                fprintf(stderr, "Semantic Error: invalid array assignment index data type\n");
                semanticErrors++;
            }

            break;

        case AST_ARRAY_ACCESS:
            if ( findExprDataType(node->son[1]) != DATATYPE_CHAR && findExprDataType(node->son[1]) != DATATYPE_INT )
            {
                fprintf(stderr, "Semantic Error: invalid array access index data type\n");
                semanticErrors++;
            }

            break;
    }

    for(i = 0; i < MAX_SONS; i++)
        check_indexing(node->son[i]);
}
/*
!(
                      node->son[0]->type == AST_ADD ||
                      node->son[0]->type == AST_SUB ||
                      node->son[0]->type == AST_MULTIPLY ||
                      node->son[0]->type == AST_DIVIDE ||
                      (node->son[0]->type == AST_SYMBOL && node->son[0]->symbol->type == ) //||
                    )
*/

void check_functions_args_and_params(AST *node)
{
    int i, endOfArgsList = 0;
    AST *currentSon;
    int function_param_data_type[MAX_FUNCTION_PARAMS];
    int function_args_data_type[MAX_FUNCTION_PARAMS]; for (i = 0; i < MAX_FUNCTION_PARAMS; i++) function_args_data_type[i] = 0;

    if (!node)
        return;

    if (node->type == AST_FUN_CALL)
    {
        currentSon = node->son[0];

        for (i = 0; i < MAX_FUNCTION_PARAMS && !endOfArgsList ; i++)
        {
            //printf("Cheguei aqui\ntipo do currentSon: ");
            if (!currentSon)
                break;

            switch(currentSon->type)
            {

                case AST_EXPR_LIST_ITEM:
                    //printf("antes da funcao\n");
                    function_args_data_type[i] = findExprDataType( currentSon->son[0] );



                    currentSon = currentSon->son[1];

                    if ( !currentSon)
                        endOfArgsList = 1;

                    else
                       currentSon = currentSon->son[0];

                    break;

                //case AST_EXPR_LIST_ITEM_NEXT:
                default: endOfArgsList = 1; break;
            }
        }

        //printf("Args para a funcao %s\n", node->symbol->text);
        //for (i = 0; i < MAX_FUNCTION_PARAMS; i++)
            //printf("%d\n", function_args_data_type[i]);

        for (i = 0; i < MAX_FUNCTION_PARAMS; i++)
            function_param_data_type[i] = node->symbol->function_datatype_params[i];

        if ( !isParamAndArgsCompatible(function_param_data_type, function_args_data_type) )
        {
            fprintf(stderr, "Semantic Error: invalid arguments for function %s\n", node->symbol->text);
            semanticErrors++;
        }
    }



    for(i = 0; i < MAX_SONS; i++)
        check_functions_args_and_params(node->son[i]);

    return;
}

void set_function_datatype_params(AST *node)
{
    AST *currentSon;
    int i = 0;

    if (!node)
        return;

    switch(node->type)
    {
        case AST_FUN_CHAR:
        case AST_FUN_INT:
        case AST_FUN_FLOAT:
            //printf("Cheguei aqui\n");

            currentSon = node->son[0];

            while(currentSon && i < MAX_FUNCTION_PARAMS)
            {
                switch(currentSon->type)
                {
                    case AST_PARAM_ITEM:
                        node->symbol->function_datatype_params[i] = currentSon->symbol->data_type;
                        //printf("%d\n", node->symbol->function_datatype_params[i]);
                        //printf("%d no parametro %d\n", node->symbol->function_datatype_params[i], i);
                        //printf("currentSon type: %d\ncurrentSon symbol data type: %d\n", currentSon->type, currentSon->symbol->data_type);
                        currentSon = currentSon->son[1];
                        i++;

                        break;

                    case AST_PARAM_ITEM_NEXT:
                        currentSon = currentSon->son[0];
                        //printf("Skipping\n");

                        break;

                    default: printf("%default: d\n", currentSon->type); break;//return;
                }

                //fprintf(stderr, "%d \n", currentSon->symbol->data_type);
            }

            //printf("Para a funcao %s:\n", node->symbol->text);

            //for(int j = 0; j < MAX_FUNCTION_PARAMS;j++)
                //printf("%d\n", node->symbol->function_datatype_params[j]);

            break;


    }



    for(i = 0; i < MAX_SONS;i++)
        set_function_datatype_params(node->son[i]);

    return;
}

int isParamAndArgsCompatible(int params[], int args[])
{
    for(int i = 0; i < MAX_FUNCTION_PARAMS; i++)
    {
        if (params[i] == 0 && args[i] == 0)
            return 1;

        if (params[i] == 0 && args[i] != 0)
            return 0;

        if (params[i] != 0 && args[i] == 0)
            return 0;

        if ( //params[i] != args[i]
            !isCompatible(params[i], args[i])
            )
            {
                //printf("Not compatible\n");
                return 0;
            }

    }
    //printf("Compatible\n");
    return 1;
}

void checkAssignments(AST *node)
{
    int i;

    if (!node)
        return;

    switch(node->type)
    {
        case AST_ASG:
            if ( !isCompatible(node->symbol->data_type, findExprDataType(node->son[0]) ) )
            {
                fprintf(stderr, "Semantic Error: invalid data type for %s assignment\n",node->symbol->text);
                semanticErrors++;
            }


            break;

        case AST_ASG_ARRAY:
            if ( !isCompatible(node->symbol->data_type, findExprDataType(node->son[1]) ) )
            {
                fprintf(stderr, "Semantic Error: invalid data type for %s assignment\n",node->symbol->text);
                semanticErrors++;
            }


            break;

        default: break;
    }

    for(i = 0; i < MAX_SONS; i++)
        checkAssignments(node->son[i]);
}

void checkFunctionReturn(AST *node)
{
    int i, answer = 0;


    if (!node)
        return;

    if (node->type == AST_CODE)
    {
        findReturnDataType( node->son[0], &answer );

        if ( !isCompatible(node->symbol->data_type, answer ) )
            {
                fprintf(stderr, "Semantic Error: invalid return data type for function %s\n",node->symbol->text);
                semanticErrors++;
            }
    }


    for(i = 0; i < MAX_SONS; i++)
        checkFunctionReturn(node->son[i]);
}

void findReturnDataType(AST *node, int *answer)
{
    int i, temp;



    if (!node)
        return;

    if (*answer == -1)
        return;

    switch(node->type)
    {
        case AST_RETURN:
            temp = findExprDataType(node->son[0]);

            if( *answer != 0 && temp != *answer)
                *answer = -1;
            else
                *answer = temp;

        default: break;
    }

    for(i = 0; i < MAX_SONS; i++)
        findReturnDataType(node->son[i], answer);
}

void setArraySize(AST *node)
{
    int i, arraySize;

    if (!node)
        return;

    switch(node->type)
    {
        case AST_ARRAY_DEC:

            arraySize = interpretExpr(node->son[0]->son[0]);

            if (arraySize == 0)
            {
                fprintf(stderr, "Semantic Error: invalid array size for %s\n", node->son[0]->symbol->text);
                semanticErrors++;
            }

            node->son[0]->symbol->array_size = arraySize;


        default: break;
    }


    for(i = 0; i < MAX_SONS; i++)
        setArraySize(node->son[i]);
}

int interpretExpr(AST *node)
{
    int i;

    //if (!node)
    //    return;


    switch(node->type)
    {
        case AST_SYMBOL: return atoi( node->symbol->text );


        case AST_ADD:
            return interpretExpr(node->son[0]) + interpretExpr(node->son[1]);

        case AST_SUB:
            return interpretExpr(node->son[0]) - interpretExpr(node->son[1]);

        case AST_MULTIPLY:
            return interpretExpr(node->son[0]) * interpretExpr(node->son[1]);

        case AST_DIVIDE:
            return interpretExpr(node->son[0]) / interpretExpr(node->son[1]);

        default: return 0;
    }
}

void checkArraySize(AST *node)
{
    int i, indexAccessed;

    if (!node)
        return;

    switch(node->type)
    {
        case AST_ARRAY_ACCESS:
            indexAccessed = interpretExpr(node->son[1]);

            if (indexAccessed >= node->son[0]->symbol->array_size)
            {
                fprintf(stderr, "Semantic Error: invalid array index for %s\n", node->son[0]->symbol->text);
                semanticErrors++;
            }


        default: break;
    }


    for(i = 0; i < MAX_SONS; i++)
        checkArraySize(node->son[i]);
}
