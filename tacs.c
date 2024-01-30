#include "tacs.h"

TAC* tacCreateIF(TAC* code0, TAC* code1);
TAC* tacCreateWHILE(TAC* code0, TAC* code1);
TAC* tacCreateCODE(TAC* code0, HASH_NODE* symbol);
TAC* tacCreatePRINT(TAC* code0);

HASH_NODE* makeLabel(void);

TAC* tacCreate(int type, HASH_NODE *result, HASH_NODE *operand1, HASH_NODE *operand2)
{
    TAC *newNode = 0;

    newNode = (TAC*) malloc(sizeof(TAC));

    newNode->type = type;
    newNode->result = result;
    newNode->operand1 = operand1;
    newNode->operand2 = operand2;
    newNode->prev = 0;
    newNode->next = 0;

    return newNode;
}

void tacPrint(TAC* node)
{
    if (!node || node->type == TAC_SYMBOL)
        return;

    fprintf(stderr, "TAC(");
    
    switch(node->type)
    {
        case TAC_SYMBOL:
            fprintf(stderr, "SYMBOL");
            break;

        case TAC_ADD:
            fprintf(stderr, "ADD");
            break;

        case TAC_SUB:
            fprintf(stderr, "SUB");
            break;

        case TAC_MULTIPLY:
            fprintf(stderr, "MULTIPLY");
            break;

        case TAC_DIVIDE:
            fprintf(stderr, "DIVIDE");
            break;

        case TAC_ASG:
            fprintf(stderr, "ASG");
            break;

        case TAC_ASG_ARRAY:
            fprintf(stderr, "ASG_ARRAY");
            break;

        case TAC_JFALSE:
            fprintf(stderr, "JFALSE");
            break;

        case TAC_LABEL:
            fprintf(stderr, "LABEL");
            break;

        case TAC_BEGINFUN:
            fprintf(stderr, "BEGINFUN");
            break;

        case TAC_ENDFUN:
            fprintf(stderr, "ENDFUN");
            break;

        case TAC_JUMP:
            fprintf(stderr, "JUMP");
            break;

        case TAC_CALL:
            fprintf(stderr, "CALL");
            break;

        case TAC_ARG:
            fprintf(stderr, "ARG");
            break;

        case TAC_RET:
            fprintf(stderr, "RET");
            break;

        case TAC_PRINT:
            fprintf(stderr, "PRINT");
            break;

        case TAC_READ:
            fprintf(stderr, "READ");
            break;

        default:
            fprintf(stderr, "UNKNOWN");
            break;
    }

    fprintf(stderr, ", %s", (node->result)? node->result->text: "0");
    fprintf(stderr, ", %s", (node->operand1)? node->operand1->text: "0");
    fprintf(stderr, ", %s", (node->operand2)? node->operand2->text: "0");

    fprintf(stderr, ")\n");
}

void tacPrintBackwards(TAC* node)
{
    if (!node)
        return;

    tacPrintBackwards(node->prev);
    tacPrint(node);
}

TAC* tacGenerateCode(AST *node)
{
    int i;
    TAC* result, *code[MAX_SONS];

    if (!node)
        return 0;

    for(i = 0; i < MAX_SONS; i++)
        code[i] = tacGenerateCode(node->son[i]);

    switch(node->type)
    {
        case AST_SYMBOL:
            result = tacCreate(TAC_SYMBOL, node->symbol, 0, 0);
            break;

        case AST_ARRAY_ACCESS:
            result = tacCreate(TAC_ARRAY_ACCESS, node->son[0]->symbol, 0, 0);


        case AST_ADD:
            result = tacJoin(tacJoin(code[0], code[1]), 
                tacCreate(TAC_ADD, makeTemp(), code[0]?code[0]->result:0, code[1]?code[1]->result:0));

            break;

        case AST_SUB:
            result = tacJoin(tacJoin(code[0], code[1]), 
                tacCreate(TAC_SUB, makeTemp(), code[0]?code[0]->result:0, code[1]?code[1]->result:0));

            break;

        case AST_MULTIPLY:
            result = tacJoin(tacJoin(code[0], code[1]), 
                tacCreate(TAC_MULTIPLY, makeTemp(), code[0]?code[0]->result:0, code[1]?code[1]->result:0));

            break;

        case AST_DIVIDE:
            result = tacJoin(tacJoin(code[0], code[1]), 
                tacCreate(TAC_DIVIDE, makeTemp(), code[0]?code[0]->result:0, code[1]?code[1]->result:0));

            break;


        case AST_ASG:
            result = tacJoin(code[0], tacCreate(TAC_ASG, node->symbol, code[0]?code[0]->result:0, 0));

            break;

        case AST_ASG_ARRAY:
            result = tacJoin(code[1], tacCreate(TAC_ASG_ARRAY, node->symbol, code[1]?code[1]->result:0, 0));

            break;

        case AST_IF:
            result = tacCreateIF(code[0], code[1]);

            break;

        case AST_IFELSE:
            result = tacCreateIF(code[0], code[1]);

            break;

        case AST_WHILE:
            result = tacCreateWHILE(code[0], code[1]);

            break;

        case AST_CODE:
            result = tacCreateCODE(code[0], node->symbol);

            break;

        case AST_PRINT_LIT:
            result = tacCreatePRINT(code[0]);

            break;

        case AST_PRINT_EXPR:
            result = tacCreatePRINT(code[0]);

            break;


        default: result = tacJoin(code[0], tacJoin(code[1], tacJoin(code[2], code[3])));
    }
    
    return result;
}

TAC* tacJoin(TAC* tacList1, TAC* tacList2)
{
    TAC* point;

    if (!tacList1)
        return tacList2;

    if (!tacList2)
        return tacList1;

    for (point = tacList2; point->prev; point = point->prev)
    ;

    point->prev = tacList1;

    return tacList2;
}

TAC* tacCreateIF(TAC* code0, TAC* code1)
{
    TAC* jumpTac = 0;
    TAC* labelTac = 0;
    HASH_NODE* newLabel = 0;

    newLabel = makeLabel();

    jumpTac = tacCreate(TAC_JFALSE, newLabel, code0?code0->result:0, 0);
    jumpTac->prev = code0;
    labelTac = tacCreate(TAC_LABEL, newLabel, 0, 0);
    labelTac->prev = code1;

    return tacJoin(jumpTac, labelTac);
}

HASH_NODE* makeLabel()
{
    static int serial = 0;
    char buffer[256] = "";

    sprintf(buffer, "mY___weirdLlllllaaabell%d", serial++);
    return hashInsert(buffer, SYMBOL_LABEL);
}

TAC* tacCreateWHILE(TAC* code0, TAC* code1)
{
    TAC* point = 0;
    TAC* jumpTac = 0;
    TAC* absJumpTac = 0;

    TAC* labelTac = 0;
    HASH_NODE* newLabel = 0;
    
    TAC* absLabelTac = 0;
    HASH_NODE* absNewLabel = 0;


    absNewLabel = makeLabel();
    newLabel = makeLabel();
    

    absLabelTac = tacCreate(TAC_LABEL, absNewLabel, 0, 0);
    labelTac = tacCreate(TAC_LABEL, newLabel, 0, 0);
    
    absJumpTac = tacCreate(TAC_JUMP, absNewLabel, 0, 0);
    jumpTac = tacCreate(TAC_JFALSE, newLabel, code0?code0->result:0, 0);
    

    
    code0 = tacJoin(absLabelTac, code0);
    jumpTac->prev = code0;

    absJumpTac->prev = code1;
    labelTac->prev = absJumpTac;

    return tacJoin(jumpTac, labelTac);
}

TAC* tacCreateCODE(TAC* code0, HASH_NODE* symbol)
{
    TAC* beginFunTac = tacCreate(TAC_BEGINFUN, symbol, 0, 0);
    TAC* endFunTac = tacCreate(TAC_ENDFUN, symbol, 0, 0);

    code0 = tacJoin(beginFunTac, code0);

    endFunTac->prev = code0;

    return endFunTac;
}

TAC* tacCreatePRINT(TAC* code0)
{
    TAC* printTac = tacCreate(TAC_PRINT, 0, code0?code0->result:0, 0);

    printTac->prev = code0;

    return printTac;
}