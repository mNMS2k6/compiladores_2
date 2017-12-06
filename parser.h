/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_PARSER_H_INCLUDED
# define YY_YY_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 71 "parser.bison" /* yacc.c:1915  */

#include <stdio.h>
#include <stdlib.h>
#include "ast.h"  

extern int yylex();
extern int yyline;
extern char* yytext;
extern FILE* yyin;
extern void yyerror(const char* msg);
funcList root;

#line 57 "parser.h" /* yacc.c:1915  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    INT = 258,
    PLUS = 259,
    SUB = 260,
    MUL = 261,
    DIV = 262,
    MOD = 263,
    EQUALS = 264,
    DIFF = 265,
    GREATER_THAN = 266,
    LESS_THAN = 267,
    GREATER_THAN_EQUALS = 268,
    LESS_THAN_EQUALS = 269,
    ATRIB = 270,
    OPEN = 271,
    CLOSE = 272,
    IF1 = 273,
    ELSE1 = 274,
    FOR1 = 275,
    TRUE = 276,
    FALSE = 277,
    TEXT = 278,
    NL = 279,
    ABRE = 280,
    FECHA = 281,
    F = 282,
    VAR = 283,
    FUNC = 284,
    INPUT = 285,
    OUTPUT = 286,
    ANDC = 287
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 47 "parser.bison" /* yacc.c:1915  */

int intValue;
Expr exprValue;
char *variable;
cmdList cmd_l; 
cmd cmd_;
func func_;
funcList funcList_;

#line 112 "parser.h" /* yacc.c:1915  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_H_INCLUDED  */
