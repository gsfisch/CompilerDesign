%{
	// Nome: Guilherme S. Fisch

	#include<stdio.h>
	int yyerror(char* s);
	#include "y.tab.h"
	#include "hash.h"


	
	int running = 1;
	int lineNumber = 1;

	int getLineNumber()
	{
		return lineNumber;
	}

	int isRunning()
	{
		return running;
	}

	void initMe()
	{

	}
%}

%x SINGLE_LINE_COMMENT
%x MULTY_LINE_COMMENT

%%

"exit"			{ return -1; }

"char"			{ return KW_CHAR; }
"int"			{ return KW_INT; }
"float"			{ return KW_FLOAT; }
"code"			{ return KW_CODE; }
"if"			{ return KW_IF; }
"else"			{ return KW_ELSE; }
"while"			{ return KW_WHILE; }
"input"			{ return KW_INPUT; }
"print"			{ return KW_PRINT; }
"return"		{ return KW_RETURN; }

"<="			{ return OPERATOR_LE; }
">="			{ return OPERATOR_GE; }
"=="			{ return OPERATOR_EQ; }
"!="			{ return OPERATOR_DIF; }

[-,;()[\]{}=+*/%<>&|~]	{ return yytext[0]; }

[0-9]*				{ yylval.symbol = hashInsert(yytext, SYMBOL_LIT_INTE);return LIT_INT; }
'[-a-zA-Z_0-9"!@#$%¨&*()_=+´`[\]{}^~\\|,<.>;:/?\n]'	{ yylval.symbol = hashInsert(yytext, SYMBOL_LIT_CARA);return LIT_CHAR; }
[a-zA-Z_0-9]*		{ yylval.symbol = hashInsert(yytext, SYMBOL_IDENTIFIER);return TK_IDENTIFIER; }
[0-9]*\.[0-9]+		{ yylval.symbol = hashInsert(yytext, SYMBOL_LIT_FLOAT);return LIT_REAL; }
\"([^\\\"]|\\.)*\"	{ yylval.symbol = hashInsert(yytext, SYMBOL_LIT_STRING);return LIT_STRING; }

"///"				{ BEGIN(MULTY_LINE_COMMENT); }
"//"				{ BEGIN(SINGLE_LINE_COMMENT); }

[ \t]			{  }
"\n"			{ ++lineNumber; }

.			{ return TOKEN_ERROR; }


<MULTY_LINE_COMMENT>"\\\\\\"		{ BEGIN(INITIAL); }
<MULTY_LINE_COMMENT>"\n"		{ ++lineNumber; }
<MULTY_LINE_COMMENT>.


<SINGLE_LINE_COMMENT>"\n"	{ ++lineNumber; BEGIN(INITIAL); }
<SINGLE_LINE_COMMENT>.

%%


int yywrap()
{
	running = 0;
	return 1;
}
