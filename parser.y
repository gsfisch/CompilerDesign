%{
	//Nome: Guilherme Schaab Fisch

#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "ast.h"
#include "semantic.h"
#include "tacs.h"

int yyerror();
int getLineNumber();
int yylex();

extern FILE* outputFile;

%}

%union
{
	struct hash_node * symbol;
	struct astnode *ast;
}

%left '&' '|'
%left OPERATOR_LE OPERATOR_GE OPERATOR_EQ OPERATOR_DIF '<' '>'
%left '+' '-'
%left '*' '/'
%right '~'


%token KW_CHAR
%token KW_INT
%token KW_FLOAT

%token KW_CODE

%token KW_IF
%token KW_ELSE
%token KW_WHILE
%token KW_INPUT
%token KW_PRINT
%token KW_RETURN

%token OPERATOR_LE
%token OPERATOR_GE
%token OPERATOR_EQ
%token OPERATOR_DIF

%token<symbol> TK_IDENTIFIER

%token<symbol> LIT_INT
%token<symbol> LIT_REAL
%token<symbol> LIT_CHAR
%token<symbol> LIT_STRING

%type<ast> expr
%type<ast> command
%type<ast> commandList
%type<ast> commandBlock
%type<ast> controlStatement
%type<ast> funcDefinition
%type<ast> codeList
%type<ast> inputArgument
%type<ast> exprList
%type<ast> exprListTail
%type<ast> exprNotEmpty
%type<ast> programa
%type<ast> globalDeclarations
%type<ast> functionDeclaration
%type<ast> funcInputType
%type<ast> parameterList
%type<ast> parameterListTail
%type<ast> parameterListNotEmpty
%type<ast> anyLiteral
%type<ast> arrayDeclaration
%type<ast> arrayInit
%type<ast> arrayStart
%type<ast> string


%token TOKEN_ERROR

%%

programa: globalDeclarations codeList			{ $$ = astCreate(AST_ROOT, 0, $1, $2, 0, 0); /*astPrint($$, 0);*/ astDecompilation($$, outputFile);
                                                    check_and_set_declaration($$); check_undeclared(); check_operands($$); check_indexing($$); set_function_datatype_params($$); check_functions_args_and_params($$);
                                                    checkAssignments($$); checkFunctionReturn($$); setArraySize($$); checkArraySize($$);
													tacPrintBackwards(tacGenerateCode($$)); }
	;

globalDeclarations: KW_CHAR TK_IDENTIFIER '=' anyLiteral ';' globalDeclarations	{ $$ = astCreate(AST_SCALAR_CHAR_DEC, $2, $4, $6, 0, 0); }
	| KW_INT TK_IDENTIFIER '=' anyLiteral ';' globalDeclarations		{ $$ = astCreate(AST_SCALAR_INT_DEC, $2, $4, $6, 0, 0); }
	| KW_FLOAT TK_IDENTIFIER '=' anyLiteral ';' globalDeclarations		{ $$ = astCreate(AST_SCALAR_FLOAT_DEC, $2, $4, $6, 0, 0); }
	| arrayDeclaration globalDeclarations					{ $$ = astCreate(AST_ARRAY_DEC, 0, $1, $2, 0, 0); }
	| functionDeclaration globalDeclarations				{ $$ = astCreate(AST_FUN_DEC, 0, $1, $2, 0, 0); }
	|									{ $$ = 0; }
	;

functionDeclaration: KW_CHAR TK_IDENTIFIER '(' parameterList ')' ';'	{ $$ = astCreate(AST_FUN_CHAR, $2, $4, 0, 0, 0); }
	| KW_INT TK_IDENTIFIER '(' parameterList ')' ';'		{ $$ = astCreate(AST_FUN_INT, $2, $4, 0, 0, 0); }
	| KW_FLOAT TK_IDENTIFIER '(' parameterList ')' ';'		{ $$ = astCreate(AST_FUN_FLOAT, $2, $4, 0, 0, 0); }


parameterList: funcInputType TK_IDENTIFIER parameterListTail		    { $$ = astCreate(AST_PARAM_ITEM, $2, $1, $3, 0, 0);
    //| funcInputType TK_IDENTIFIER '[' ']' parameterListTail		        { $$ = astCreate(AST_PARAM_ITEM_ARRAY, $2, $1, $5, 0, 0); }
    }
	|								                                    { $$ = 0; }
	;

parameterListTail: ',' parameterListNotEmpty					        { /*$$ = $2;*/ $$ = astCreate(AST_PARAM_ITEM_NEXT, 0, $2, 0, 0, 0); }
	|								                                    { $$ = 0; }
	;

parameterListNotEmpty: funcInputType TK_IDENTIFIER parameterListTail    { $$ = astCreate(AST_PARAM_ITEM, $2, $1, $3, 0, 0);
    //| funcInputType TK_IDENTIFIER '[' ']' parameterListTail		        { $$ = astCreate(AST_PARAM_ITEM_ARRAY, $2, $1, $5, 0, 0); }
    }
    ;

funcInputType: KW_CHAR					{ $$ = astCreate(AST_PARAM_TYPE_CHAR, 0, 0, 0, 0, 0); }
	| KW_INT							{ $$ = astCreate(AST_PARAM_TYPE_INT, 0, 0, 0, 0, 0); }
	| KW_FLOAT							{ $$ = astCreate(AST_PARAM_TYPE_FLOAT, 0, 0, 0, 0, 0); }
	;

anyLiteral: LIT_INT							{ $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); }
	| LIT_REAL							{ $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); }
	| LIT_CHAR							{ $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); }
	| LIT_STRING							{ $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); }
	;

arrayDeclaration: KW_CHAR TK_IDENTIFIER '[' expr ']' arrayInit ';'	{ $$ = astCreate(AST_ARRAY_CHAR_INIT, $2, $4, $6, 0, 0); }
	| KW_CHAR TK_IDENTIFIER '[' expr ']' ';'	            		{ $$ = astCreate(AST_ARRAY_CHAR, $2, $4, 0, 0, 0); }
	| KW_INT TK_IDENTIFIER '[' expr ']' arrayInit ';'		        { $$ = astCreate(AST_ARRAY_INT_INIT, $2, $4, $6, 0, 0); }
	| KW_INT TK_IDENTIFIER '[' expr ']' ';'				            { $$ = astCreate(AST_ARRAY_INT, $2, $4, 0, 0, 0); }
	| KW_FLOAT TK_IDENTIFIER '[' expr ']' arrayInit ';'		        { $$ = astCreate(AST_ARRAY_FLOAT_INIT, $2, $4, $6, 0, 0); }
	| KW_FLOAT TK_IDENTIFIER '[' expr ']' ';'			            { $$ = astCreate(AST_ARRAY_FLOAT, $2, $4, 0, 0, 0); }
	;


arrayInit: expr arrayInit				{ $$ = astCreate(AST_ARRAY_INIT_ITEM, 0, $1, $2, 0, 0); }
	|						            { $$ = 0; }
	;


codeList: funcDefinition codeList		{ $$ = astCreate(AST_FUN_DEF, 0, $1, $2, 0, 0); }
	|						            { $$ = 0; }
	;

funcDefinition: KW_CODE TK_IDENTIFIER command		{ $$ = astCreate(AST_CODE, $2, $3, 0, 0, 0); }
	;

command: TK_IDENTIFIER '=' expr ';'			        { $$ = astCreate(AST_ASG, $1, $3, 0, 0, 0); }
	| TK_IDENTIFIER '[' expr ']' '=' expr ';'	    { $$ = astCreate(AST_ASG_ARRAY, $1, $3, $6, 0, 0); }
	| commandBlock					                { $$ = $1; }
	| KW_PRINT string ';'			                { $$ = astCreate(AST_PRINT_LIT, 0, $2, 0, 0, 0); }
	| KW_PRINT expr ';'				                { $$ = astCreate(AST_PRINT_EXPR, 0, $2, 0, 0, 0); }
	| KW_RETURN expr ';'				            { $$ = astCreate(AST_RETURN, 0, $2, 0, 0, 0); }
	| controlStatement				                { $$ = $1; }
	| ';'						                    { $$ = 0; }
	;

string: LIT_STRING                  { $$ = astCreate(AST_STRING, $1, 0, 0, 0, 0); }
    ;

controlStatement: KW_IF '(' expr ')' command		{ $$ = astCreate(AST_IF, 0, $3, $5, 0, 0); }
	| KW_IF '(' expr ')' command KW_ELSE command	{ $$ = astCreate(AST_IFELSE, 0, $3, $5, $7, 0); }
	| KW_WHILE '(' expr ')' command			{ $$ = astCreate(AST_WHILE, 0, $3, $5, 0, 0); }
	;

commandBlock: '{' commandList '}'		{ /*$$ = $2;*/ $$ = astCreate(AST_CMDB, 0, $2, 0, 0, 0); }
	;

commandList: command commandList		{ $$ = astCreate(AST_CMDL, 0, $1, $2, 0, 0); }
	|					{ $$ = 0; }
	;

expr: LIT_INT					        { $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); }
	| LIT_REAL				            { $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); }
	| LIT_CHAR				            { $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); }
	| arrayStart '[' expr ']'		    { $$ = astCreate(AST_ARRAY_ACCESS, 0, $1, $3, 0, 0); }
	| TK_IDENTIFIER				        { $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); }
	| expr '+' expr				        { $$ = astCreate(AST_ADD, 0, $1, $3, 0, 0); }
	| expr '-' expr				        { $$ = astCreate(AST_SUB, 0, $1, $3, 0, 0); }
	| expr '*' expr				        { $$ = astCreate(AST_MULTIPLY, 0, $1, $3, 0, 0); }
	| expr '/' expr				        { $$ = astCreate(AST_DIVIDE, 0, $1, $3, 0, 0); }
	| expr '<' expr				        { $$ = astCreate(AST_LT, 0, $1, $3, 0, 0); }
	| expr '>' expr				        { $$ = astCreate(AST_GT, 0, $1, $3, 0, 0); }
	| expr OPERATOR_LE expr			    { $$ = astCreate(AST_LE, 0, $1, $3, 0, 0); }
	| expr OPERATOR_GE expr			    { $$ = astCreate(AST_GE, 0, $1, $3, 0, 0); }
	| expr OPERATOR_EQ expr			    { $$ = astCreate(AST_EQ, 0, $1, $3, 0, 0); }
	| expr OPERATOR_DIF expr		    { $$ = astCreate(AST_DIF, 0, $1, $3, 0, 0); }
	| expr '&' expr				        { $$ = astCreate(AST_AND, 0, $1, $3, 0, 0); }
	| expr '|' expr				        { $$ = astCreate(AST_OR, 0, $1, $3, 0, 0); }
	| '~' expr				            { $$ = astCreate(AST_NOT, 0, $2, 0, 0, 0); }
	| '(' expr ')'				        { $$ = $2; }
	| KW_INPUT '(' inputArgument ')'	{ $$ = astCreate(AST_INPUT, 0, $3, 0, 0, 0); }
	| TK_IDENTIFIER '(' exprList ')'	{ $$ = astCreate(AST_FUN_CALL, $1, $3, 0, 0, 0); }
	;

arrayStart: TK_IDENTIFIER			{ $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); }
	;


inputArgument: KW_CHAR				{ $$ = astCreate(AST_INPUT_ARG_CHAR, 0, 0, 0, 0, 0); }
	| KW_INT				{ $$ = astCreate(AST_INPUT_ARG_INT, 0, 0, 0, 0, 0); }
	| KW_FLOAT				{ $$ = astCreate(AST_INPUT_ARG_FLOAT, 0, 0, 0, 0, 0); }
	;


exprList: expr exprListTail			{ $$ = astCreate(AST_EXPR_LIST_ITEM, 0, $1, $2, 0, 0); }
	|					{ $$ = 0; }
	;

exprListTail: ',' exprNotEmpty 		{ $$ = astCreate(AST_EXPR_LIST_ITEM_NEXT, 0, $2, 0, 0, 0); }
	|					{ $$ = 0; }
	;

exprNotEmpty: expr exprListTail			{ $$ = astCreate(AST_EXPR_LIST_ITEM, 0, $1, $2, 0, 0); }
	;

%%

int yyerror (char *s)
{
	fprintf(stderr, "Syntax error at line %d.\n", getLineNumber());
	exit(3);
}

