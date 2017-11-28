
// Tokens
%token 
INT					// Inteiros
PLUS 				// +
SUB		   			// -
MUL 				// x
DIV		  			// /
MOD 			  	// %
EQUALS 	  			// ==
DIFF 		  		// !=
GREATER_THAN 		// >
LESS_THAN 			// < 
GREATER_THAN_EQUALS // >=
LESS_THAN_EQUALS 	// <=
ATRIB 				// :=
OPEN 				// (
CLOSE 				// )
IF1 				// IF
ELSE1 				// ELSE
FOR1 				// FOR
TRUE 				// TRUE 
FALSE 				// FALSE 
TEXT 				// Variaveis 
NL 					// nova linha
ABRE 				// { 
FECHA 				// }
F 					//;
VAR 				// "var"
FUNC				// "func"
INPUT				// "fmt.Scan"
OUTPUT				// "fmt.Println"
ANDC				// "&"



// Operator associativity & precedence
%left PLUS SUB
%left MUL DIV MOD


// Root-level grammar symbol
%start program;

// Types/values in association to grammar symbols.
%union 
{
int intValue;
Expr exprValue;
char *variable;
cmdList cmd_l; 
cmd cmd_;
func func_;
funcList funcList_;
}

%type <func_> FUNCTION
%type <funcList_> FUNCTIONLIST

%type <intValue> INT
%type <exprValue> expr
%type <variable> TEXT
%type <cmd_> for_atrib
%type <cmd_> exprCmd
%type <cmd_l> cmd_list


// Use "%code requires" to make declarations go
// into both parser.c and parser.h
%code requires
{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"  

extern int yylex();
extern int yyline;
extern char* yytext;
extern FILE* yyin;
extern void yyerror(const char* msg);
funcList root;
}

%%
program: FUNCTIONLIST { root = $1; }

FUNCTIONLIST:
	FUNCTION FUNCTIONLIST { $$ = astFunction($1,$2); }

	| FUNCTION { $$ = astFunction($1, NULL); }


	
FUNCTION:
	FUNC TEXT OPEN CLOSE ABRE cmd_list FECHA { $$ = astFunc($2,$6); }
	

cmd_list: 
	exprCmd { $$ = mklist($1, NULL); }
	| exprCmd cmd_list { $$ = mklist($1,$2); }


expr: 
	INT { $$ = ast_integer($1); }
	| TEXT  { $$ = ast_var($1); }
	| TRUE { $$ = ast_integer(1);}
	| FALSE { $$ = ast_integer(0);}
	| expr PLUS expr { $$ = ast_operation(PLUS, $1, $3); }
	| expr SUB expr { $$ = ast_operation(SUB, $1, $3); }
	| expr MUL expr { $$ = ast_operation(MUL, $1, $3); }
	| expr DIV expr { $$ = ast_operation(DIV, $1, $3); }
	| expr MOD expr { $$ = ast_operation(MOD, $1, $3); }      
	| expr EQUALS expr { $$ = ast_boolean(EQUALS, $1, $3); }
	| expr DIFF expr { $$ = ast_boolean(DIFF, $1, $3); }
	| expr GREATER_THAN expr { $$ = ast_boolean(GREATER_THAN, $1, $3); }
	| expr LESS_THAN expr { $$ = ast_boolean(LESS_THAN, $1, $3); }
	| expr GREATER_THAN_EQUALS expr { $$ = ast_boolean(GREATER_THAN_EQUALS, $1, $3); }
	| expr LESS_THAN_EQUALS expr { $$ = ast_boolean(LESS_THAN_EQUALS, $1, $3); }


exprCmd:
	TEXT ATRIB expr F { $$ = mkAtrib($1,$3); }
	| VAR TEXT TEXT F { $$ = mkAtrib($2, ast_integer(0)); }
	| VAR TEXT ATRIB expr F { $$ = mkAtrib($2,$4); }
	| VAR TEXT F { $$ = mkAtrib($2, ast_integer(0)); }
	| IF1 expr ABRE cmd_list FECHA  { $$ = astIf($2, $4, NULL); }
	| IF1 expr ABRE cmd_list FECHA ELSE1 ABRE cmd_list FECHA { $$ = astIf($2, $4, $8); }
	| FOR1 for_atrib F expr F for_atrib ABRE cmd_list FECHA { $$ = astFor1($2, $4, $6, $8); }
	| FOR1 expr ABRE cmd_list FECHA { $$ = astWhile1($2, $4); }
	| INPUT OPEN ANDC TEXT CLOSE F { $$ = ast_input($4); }

for_atrib:
	TEXT ATRIB expr     { $$ = mkAtrib($1,$3); }

%%

void yyerror(const char* err) {
printf("Line %d: %s - '%s' \n", yyline, err, yytext  );
}