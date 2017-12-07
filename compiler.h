int countVar = 0;
int countLabel = 1;
//Stack stack = NULL;

char* newVar()
{
  int temp;
  char c = 't';
  if(countVar > 9)
  {
    temp = countVar - 9;
    c = 's';
  }
  else if(countVar > 16)
  {
    countVar = 0;
    temp = countVar;
  }
  else
    temp = countVar;

  char *var = malloc(sizeof(int) + 2);     
  sprintf(var, "$%c%d", c, temp);
  
  countVar++;  
  return var;
}

char* newLabel()
{
  char *label = malloc(sizeof(int) + 5);     
  sprintf(label, "label%d", countLabel);
  
  countLabel++;  

  return label;
}

Pair Expressions(Expr expr) 
{
  if (expr->kind == E_INTEGER) 
  {
    return mkPair(mkInt(expr->attr.value), NULL);  
  } 
  
  else if (expr->kind == E_VARIABLE) 
  {
    if(mkStackContains(expr->attr.var, stack) == 0)
     stack = mkStack(expr->attr.var, stack);

   char *var = newVar();

   Instr inst = mkInstr(C_LW, mkVar(var), mkVar(expr->attr.var), NULL);

   return mkPair(mkVar(var), mkInstrList(inst, NULL));
 }

 else if (expr->kind == E_OPERATION) 
 {
  Pair left = Expressions(expr->attr.op.left);
  Pair right = Expressions(expr->attr.op.right);

      // add $v, int, $v
      // slt $v, $v, int
      // casos erro
  if(left->name->kind == C_INT)
  {
   Instr inst = mkInstr(C_LI, mkVar(newVar()), left->name, NULL);

   left->name = inst->addr1;
   left->list = mkInstrList(inst, NULL);
 }

 if(right->name->kind == C_INT)
 {
   Instr inst = mkInstr(C_LI, mkVar(newVar()), right->name, NULL);

   right->name = inst->addr1;
   right->list = mkInstrList(inst, NULL);
 }

 char *var = newVar();

 OpKind op;

 switch(expr->attr.op.operator) 
 {
   case PLUS: 
   op = C_ADD;
   break;

   case SUB: 
   op = C_SUB;
   break;

   case DIV: 
   op = C_DIV;
   break;

   case MUL: 
   op = C_MUL;
   break;

   case EQUALS: 
   op = C_SEQ;
   break;

   case DIFF: 
   op = C_SNE;
   break;

   case GREATER_THAN: 
   op = C_SGT;
   break;

   case LESS_THAN: 
   op = C_SLT;
   break;

   case GREATER_THAN_EQUALS: 
   op = C_SGE;
   break;

   case LESS_THAN_EQUALS: 
   op = C_SLE;
   break;
 }

 Instr inst = mkInstr(op, mkVar(var), left->name, right->name);

 InstrList list = append(left->list, right->list);

      // Instr -> InstrList só com um argumento -> append das duas listas
 list = append(list, mkInstrList(inst, NULL));

 return mkPair(mkVar(var), list);
}

else if (expr->kind == E_OPBOOLEAN)
{

  Pair left = Expressions(expr->attr.op.left);
  Pair right = Expressions(expr->attr.op.right);

  if(left->name->kind == C_INT)
  {
   Instr inst = mkInstr(C_LI, mkVar(newVar()), left->name, NULL);

   left->name = inst->addr1;
   left->list = mkInstrList(inst, NULL);
 }

 if(right->name->kind == C_INT)
 {
   Instr inst = mkInstr(C_LI, mkVar(newVar()), right->name, NULL);

   right->name = inst->addr1;
   right->list = mkInstrList(inst, NULL);
 }

 char *label = newLabel();

 OpKind op;

 switch(expr->attr.op.operator) 
 {
   case EQUALS: 
   op = C_BNE;
   break;

   case DIFF: 
   op = C_BEQ;
   break;

   case GREATER_THAN: 
   op = C_BLE;
   break;

   case LESS_THAN: 
   op = C_BGE;
   break;

   case GREATER_THAN_EQUALS: 
   op = C_BLT;
   break;

   case LESS_THAN_EQUALS: 
   op = C_BGT;
   break;
 }

 Instr inst = mkInstr(op, left->name, right->name, mkVar(label));
 InstrList list = append(left->list, right->list);

      // Instr -> InstrList só com um argumento -> append das duas listas
 list = append(list, mkInstrList(inst, NULL));

 return mkPair(NULL, list);
}
  /*
    else if (expr->kind == E_BRACKETS) 
    {
    return Expressions(expr->attr.exp);
    }
  */

  /* else if (expr->kind == E_INPUT) 
     {
     Instr inst = mkInstr(C_LI, mkVar("$v0"), mkInt(5), NULL);
     InstrList list = NULL;
     list = append(list, mkInstrList(inst, NULL));

     inst = mkInstr(C_SYSCALL, NULL, NULL, NULL);
     list = append(list, mkInstrList(inst, NULL));

     inst = mkInstr(C_MOVE, mkVar(newVar()), mkVar("$v0"), NULL);
     list = append(list, mkInstrList(inst, NULL));

     return mkPair(inst->addr1, list);
     } */
return NULL;
}

InstrList Commands(cmd command) 
{
  countVar = 0;
  if (command->kind == E_ATRIB) 
  {
    if(mkStackContains(command->attr.atrib.var, stack) == 0)
     stack = mkStack(command->attr.atrib.var, stack);

   Pair exp = Expressions(command->attr.atrib.exp);

      // nao fazer sw de int
   if(exp->name->kind == C_INT)
   {
     Instr inst = mkInstr(C_LI, mkVar(newVar()), exp->name, NULL);

     exp->name = inst->addr1;
     exp->list = mkInstrList(inst, NULL);
   }

   Instr inst = mkInstr(C_SW, exp->name, mkVar(command->attr.atrib.var), NULL);

      // lado direito do para ++ nova instr que passou a instrList
   return append(exp->list, mkInstrList(inst, NULL));
 } 

 else if(command->kind == E_IF)
 {
      // IF
  char *exitLabel = newLabel();

      // boolean condition
  Pair cond = Expressions(command->attr.iff.cond); 

  InstrList list = NULL;
  Address label = last(cond->list)->addr3;

      if(command->attr.iff.elsee == NULL)// && command->attr.iff.elseiff == NULL)
       last(cond->list)->addr3 = mkVar(exitLabel);

     list = append(list, cond->list);

      // body
     cmdList cmd = command->attr.iff.body; 
     while(cmd!=NULL)
     { 
       list = append(list, Commands(cmd->head));
       cmd = cmd->tail;
     }

     Instr inst;
      if(/*command->attr.iff.elseiff != NULL ||*/ command->attr.iff.elsee != NULL)
     {
       inst = mkInstr(C_J, mkVar(exitLabel), NULL, NULL);
       list = append(list, mkInstrList(inst, NULL));
     }


      // ELSE
     if(command->attr.iff.elsee != NULL)
     {
       inst = mkInstr(C_LABEL, label, NULL, NULL);
       list = append(list, mkInstrList(inst, NULL));

	  // body
       cmd = command->attr.iff.elsee;
       while(cmd != NULL)
       { 
         list = append(list, Commands(cmd->head));
         cmd = cmd->tail;
       }
     }

     inst = mkInstr(C_LABEL, mkVar(exitLabel), NULL, NULL);
     list = append(list, mkInstrList(inst, NULL));
     return list;
   }

   else if(command->kind == E_WHILE1)
   {
    char *startLabel = newLabel();
    
    InstrList list = NULL;
    Instr inst = mkInstr(C_LABEL, mkVar(startLabel), NULL, NULL);
    list = append(list, mkInstrList(inst, NULL));

      // boolean condition (if)
    Pair cond = Expressions(command->attr.while1.cond); 

    Address exitLabel = last(cond->list)->addr3;
    list = append(list, cond->list);

      // body
    cmdList cmd = command->attr.while1.body; 
    while(cmd != NULL)
    { 
     list = append(list, Commands(cmd->head));
     cmd = cmd->tail;
   }

   inst = mkInstr(C_J, mkVar(startLabel), NULL, NULL);
   list = append(list, mkInstrList(inst, NULL));

   inst = mkInstr(C_LABEL, exitLabel, NULL, NULL);
   list = append(list, mkInstrList(inst, NULL));

   return list;
 }

 else if(command->kind == E_FOR1)
 {
      // add to list left side
  InstrList list = Commands(command->attr.for1.decl);


  char *startLabel = newLabel();

      // variavel de comparação
  Address startVar = last(list)->addr2;

      // new label
  Instr inst = mkInstr(C_LABEL, mkVar(startLabel), NULL, NULL);
  list = append(list, mkInstrList(inst, NULL));

  // criar o if
  cmd ifcommand = astIf(command->attr.for1.cond, NULL, NULL);

  list = append(list, Commands(ifcommand));

  // apagar a ultima linha do if (sem else) (jump)
  list = removeLast(list);

      // buscar o label final (if)
  Address exitLabel = last(list)->addr3;

      // body
  cmdList cmmd = command->attr.for1.body; 
  
  while(cmmd!=NULL)
  { 
   list = append(list, Commands(cmmd->head));
   cmmd = cmmd->tail;
 }

      // incrementar var
      /*
      Expr increment = ast_operation(PLUS, ast_var(startVar->content.var), ast_integer(1));
      cmd incrementcmd = mkAtrib(startVar->content.var, increment);
      */
 list = append(list, Commands(command->attr.for1.incre));

      // salta para o inicio
 inst = mkInstr(C_J, mkVar(startLabel), NULL, NULL);
 list = append(list, mkInstrList(inst, NULL));

 inst = mkInstr(C_LABEL, exitLabel, NULL, NULL);
 list = append(list, mkInstrList(inst, NULL));

 return list;
}

  /*  else if(command->kind == E_OUTPUT)
      {
      InstrList list = NULL;
      Pair p = Expressions(command->attr.output);

      list = append(list, p->list);
      Instr inst = mkInstr(C_LI, mkVar("$v0"), mkInt(1), NULL);
      list = append(list, mkInstrList(inst, NULL));

      if(p->name->kind == C_INT)
      {
      Instr inst = mkInstr(C_LI, mkVar(newVar()), p->name, NULL);
      list = append(list, mkInstrList(inst, NULL));
      p->name = inst->addr1;
      }

      inst = mkInstr(C_MOVE, mkVar("$a0"), p->name, NULL);
      list = append(list, mkInstrList(inst, NULL));

      inst = mkInstr(C_SYSCALL, NULL, NULL, NULL);
      list = append(list, mkInstrList(inst, NULL));
  
    
      return list;
      } */
else if(command->kind == E_INPUT)
{
  InstrList list = NULL;
  Pair p = Expressions(command->attr.input.cond);

  list = append(list, p->list);
  Instr inst = mkInstr(C_LI, mkVar("$v0"), mkInt(1), NULL);
  list = append(list, mkInstrList(inst, NULL));

  if(p->name->kind == C_INT)
  {
   Instr inst = mkInstr(C_LI, mkVar(newVar()), p->name, NULL);
   list = append(list, mkInstrList(inst, NULL));
   p->name = inst->addr1;
 }

 inst = mkInstr(C_MOVE, mkVar("$a0"), p->name, NULL);
 list = append(list, mkInstrList(inst, NULL));

 inst = mkInstr(C_SYSCALL, NULL, NULL, NULL);
 list = append(list, mkInstrList(inst, NULL));


 return list;
}
return NULL;
}
