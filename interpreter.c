#include <stdio.h>
#include "parser.h"


void eval(Expr expr)
{

  int result = 0;
  if (expr->kind == E_INTEGER)
  {
    printf("%d", expr->attr.value);
  } 
  else if (expr->kind == E_VARIABLE)
  {
    printf("%s", expr->attr.var);
  } 
  else if (expr->kind == E_INPUT)
  {
    printf("fmt.Scan(&%s)\n", expr->attr.var);
  }
  else if (expr->kind == E_OPERATION)
  {
    eval(expr->attr.op.left);

    switch (expr->attr.op.operator)
    {
      case PLUS: 
      printf(" + ");
      break;

      case SUB:
      printf(" - ");      
      break;

      case MUL:
      printf(" * ");
      break;

      case DIV:
      printf(" / ");
      break;

      case MOD:
      printf(" %% ");
      break;
      // TODO Other cases here ...
      default: yyerror("Unknown operator!");
    }
    eval(expr->attr.op.right);

  }
  
  else if (expr->kind == E_OPBOOLEAN)
  {
    eval(expr->attr.op_bl.left);

    switch (expr->attr.op_bl.operator)
    {
      case LESS_THAN: 
      printf(" < ");
      break;

      case LESS_THAN_EQUALS:
      printf(" <= ");      
      break;

      case GREATER_THAN:
      printf(" > ");
      break;

      case GREATER_THAN_EQUALS:
      printf(" >= ");
      break;

      case EQUALS:
      printf(" == ");
      break;

      case DIFF:
      printf(" != ");
      break;

      default: yyerror("Unknown operator!");
    }
    eval(expr->attr.op_bl.right);
  }

}

void cmd_list_for(cmd cmd)
{
  if (cmd->kind == E_ATRIB) 
  {
    printf("%s = ", cmd->attr.atrib.var); 
    eval(cmd->attr.atrib.exp);
  }  
}

void cmd_list(cmd cmd, int tab) 
{
  int i = 0;
  for(i=0; i<tab; i++)
  {
    printf("\t");
  }

  if (cmd->kind == E_ATRIB) 
  {
    printf("%s = ", cmd->attr.atrib.var); 
    eval(cmd->attr.atrib.exp);
    printf("\n");
  } 
  else if (cmd->kind == E_IF) 
  {
    if (cmd->attr.iff.elsee == NULL)
    { 
      printf("IF "); 
      eval(cmd->attr.iff.cond);
      printf("\n");
      for(i=0; i<tab; i++)
      {
        printf("\t");
      }
      printf("{\n");
      while(cmd->attr.iff.body != NULL)
      {
        cmd_list(cmd->attr.iff.body->head, tab+1);
        cmd->attr.iff.body = cmd->attr.iff.body->tail;
      }
      for(i=0; i<tab; i++)
      {
        printf("\t");
      }
      printf("}\n");
    }
    else
    {
      printf("IF "); 
      eval(cmd->attr.iff.cond);
      printf("\n");
      for(i=0; i<tab; i++)
      {
        printf("\t");
      }
      printf("{\n");
      while(cmd->attr.iff.body != NULL)
      {
        cmd_list(cmd->attr.iff.body->head, tab+1);
        cmd->attr.iff.body = cmd->attr.iff.body->tail;
      }
      for(i=0; i<tab; i++)
      {
        printf("\t");
      }
      printf("}\n");
      for(i=0; i<tab; i++)
      {
        printf("\t");
      }
      printf("ELSE \n");
      for(i=0; i<tab; i++)
      {
        printf("\t");
      }
      printf("{\n");
      while(cmd->attr.iff.elsee != NULL)
      {
        cmd_list(cmd->attr.iff.elsee->head, tab+1);
        cmd->attr.iff.elsee = cmd->attr.iff.elsee->tail;
      }
      for(i=0; i<tab; i++)
      {
        printf("\t");
      }
      printf("}\n");
    }
  }
  else if(cmd->kind == E_FOR1)
  {
    printf("FOR ");
    cmd_list_for(cmd->attr.for1.decl);
    printf("; ");
    eval(cmd->attr.for1.cond);
    printf("; ");
    cmd_list(cmd->attr.for1.incre, 0);
    for(i=0; i<tab; i++)
    {
      printf("\t");
    }
    printf("{\n");
    while(cmd->attr.for1.body != NULL)
    {
      cmd_list(cmd->attr.for1.body->head, tab+1);
      cmd->attr.for1.body = cmd->attr.for1.body->tail;
    }
    for(i=0; i<tab; i++)
    {
      printf("\t");
    }
    printf("}\n");
  }
  else if(cmd->kind == E_WHILE1)
  {
    printf("FOR ");
    eval(cmd->attr.while1.cond);
    printf("\n");
    for(i=0; i<tab; i++)
    {
      printf("\t");
    }
    printf("{\n");
    while(cmd->attr.while1.body != NULL)
    {
      cmd_list(cmd->attr.while1.body->head, tab+1);
      cmd->attr.while1.body = cmd->attr.while1.body->tail;
    }
    for(i=0; i<tab; i++)
    {
      printf("\t");
    }
    printf("}\n");
  }
 
  else if(cmd->kind == E_INPUT)
  {
    printf("fmt.Scan (&");
    eval(cmd->attr.input.cond);
    printf(")\n");
  }
}

void functions(func one)
{
  printf("func %s()\n{\n",one->var);
  while(one->body != NULL)
  {
    cmd_list(one->body->head,1);
    one->body = one->body->tail;
  }
  printf("}\n");
}

int main(int argc, char** argv) 
{
  --argc; ++argv;
  if (argc != 0)
  {
    yyin = fopen(*argv, "r");
    if (!yyin) {
      printf("'%s': could not open file\n", *argv);
      return 1;
    }
  } 
  if (yyparse() == 0) {

    while(root != NULL)
    {
      functions(root->head);
      root = root->tail;
    }
  }
  return 0;
}
