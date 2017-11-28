// AST constructor functions

#include <stdlib.h> // for malloc
#include "ast.h" // AST header

Expr ast_integer(int v)
{
  Expr node = (Expr) malloc(sizeof(Expr));
  node->kind = E_INTEGER;
  node->attr.value = v;
  return node;
}

Expr ast_var(char *v)
{
  Expr node = (Expr) malloc(sizeof(Expr));
  node->kind = E_VARIABLE;
  node->attr.var = v;
  return node;
}

Expr ast_operation(int operator, Expr left, Expr right)
{
  Expr node = (Expr) malloc(sizeof(struct _Expr));
  node->kind = E_OPERATION;
  node->attr.op.operator = operator;
  node->attr.op.left = left;
  node->attr.op.right = right;
  return node;
}

Expr ast_boolean (int operator, Expr left, Expr right) 
{
  Expr node = (Expr) malloc(sizeof(struct _Expr));
  node->kind = E_OPBOOLEAN;
  node->attr.op_bl.operator = operator;
  node->attr.op_bl.left = left;
  node->attr.op_bl.right = right;
  return node;
}

cmd mkAtrib(char *var, Expr e)
{
	cmd node = (cmd) malloc(sizeof(struct _cmd));
  node->kind = E_ATRIB;
  node->attr.atrib.var = var;
  node->attr.atrib.exp = e;
  return node;		
}

cmd astIf(Expr ifCond, cmdList ifBody, cmdList elsee)
{
  cmd node = (cmd) malloc(sizeof(struct _cmd));
  node->kind = E_IF;
  node->attr.iff.cond = ifCond;
  node->attr.iff.body = ifBody;
  node->attr.iff.elsee = elsee;
  return node;     
}

cmd astFor1(cmd decl, Expr cond, cmd incre, cmdList body)
{
  cmd node = (cmd) malloc(sizeof(struct _cmd));
  node->kind = E_FOR1;
  node->attr.for1.decl = decl; 
  node->attr.for1.cond = cond;
  node->attr.for1.incre = incre;
  node->attr.for1.body = body;
  return node;     
}

cmd astWhile1(Expr cond, cmdList body)
{
  cmd node = (cmd) malloc(sizeof(struct _cmd));
  node->kind = E_WHILE1;
  node->attr.while1.cond = cond;
  node->attr.while1.body = body;
  return node;     
}

cmdList mklist(cmd head, cmdList tail)
{
	cmdList node = (cmdList) malloc(sizeof(struct _cmdList));
  node->head = head;
  node->tail = tail;
  return node;	
}

cmd ast_input(char *v)
{
  cmd node = (cmd) malloc(sizeof(struct _cmd));  
  node->kind = E_INPUT;
  node->attr.input.cond = ast_var(v); 
  return node;       
}

func astFunc(char * func_n, cmdList body)
{
  func node = (func) malloc(sizeof(struct _func));
  node->var = func_n;
  node->body = body;
  return node;      
}

funcList astFunction(func one, funcList others)
{
  funcList node = (funcList) malloc(sizeof(struct _funcList));
  node->head = one;
  node->tail = others;
  return node;  
}