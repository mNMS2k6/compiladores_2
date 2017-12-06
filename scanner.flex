%{
// HEADERS
#include <stdlib.h>
#include "parser.h"

// variables maintained by the lexical analyser
int yyline = 1;
%}

%option noyywrap

%%

"//".*\n { yyline++; }

"//".* { yyline++; }

[\n]+ { yyline++; }

[ \t]+ {  }

"+" { return PLUS; }
"-" { return SUB; }
"*" { return MUL; }
"/" { return DIV; }
"%" { return MOD; }
"==" { return EQUALS; }
"!=" { return DIFF; }
">"  { return GREATER_THAN; }
"<" { return LESS_THAN; }
">=" { return GREATER_THAN_EQUALS; }
"<=" { return LESS_THAN_EQUALS; }
"true" { return TRUE; }
"false" { return FALSE; }
":=" { return ATRIB; }
"(" { return OPEN; }
")" { return CLOSE; }
"{" { return ABRE; }
"}" { return FECHA; }
"if" { return IF1; }
"var" { return VAR; }
"else" { return ELSE1; }
"for" { return FOR1; }
";" { return F; }
"func" { return FUNC; }
"fmt.Scan" { return INPUT; }
"fmt.Println" { return OUTPUT; }
"&" { return ANDC; }



\-?[0-9]+ { yylval.intValue = atoi(yytext); return INT; }

\-?[a-z,A-Z]+ { yylval.variable = strdup(yytext); return TEXT; }

. { yyerror("unexpected character"); }
%%