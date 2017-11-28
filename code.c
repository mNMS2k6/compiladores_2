#include <string.h> 
#include <stdio.h>

#include <stdlib.h> // for malloc
#include "code.h" // AST header

Address mkInt(int i)
{
	Address node = (Address) malloc(sizeof(struct _Address));
	node->kind = C_INT;
	node->content.val = i;
	return node;
}

Address mkVar(char *v)
{
	Address node = (Address) malloc(sizeof(struct _Address));
	node->kind = C_VAR;
	node->content.var = v;
	return node;
}

