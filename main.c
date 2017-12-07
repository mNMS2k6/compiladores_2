#include <stdio.h>
#include "parser.h"
#include "code.h"

Stack stack = NULL;
FILE *f;

#include "printer.h"
#include "compiler.h"

InstrList listToPrint = NULL;

int main(int argc, char** argv) 
{
  --argc; ++argv;
  if (argc != 0) 
  {
    yyin = fopen(*argv, "r");
    if (!yyin) 
    {
      printf("'%s': could not open file\n", *argv);
      return 1;
    }
  }
  
  if (yyparse() == 0) 
  {
    f = fopen("mips.txt", "w");
    
    cmdList newroot = root->head->body;

    while(newroot != NULL)
    {

      countVar = 0;
      listToPrint = append(listToPrint, Commands(newroot->head));
      Instr inst = mkInstr(C_SPACE, NULL, NULL, NULL);
      listToPrint = append(listToPrint, mkInstrList(inst, NULL));

      newroot = newroot->tail;
    }
    
    printStack(stack);
    printInstrList(listToPrint);
    printExit();
    
    fclose(f);
  }

  return 0;
}
