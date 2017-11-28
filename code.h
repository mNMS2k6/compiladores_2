#ifndef __code_h__
#define __code_h__

typedef enum
  {
    C_INT,
    C_VAR,
  } AddrKind; 

typedef struct _Address
{
  AddrKind kind;

  union
  {
    int val;
    char *var;
  } content;
  
} *Address;

typedef enum
  {
    C_ADD, // add $t0 = $t1 + $t2 (add	$t0,$t1,$t2)
    C_SUB, // sub $t2 = $t3 Ð $t4 (sub	$t2,$t3,$t4)
    C_DIV, // Lo = $t5 / $t6 Hi = $t5 mod $t6 (div	$t5,$t6)
    C_MUL, /* multiply 32-bit quantities in $t3 and $t4, and store 64-bit
			  result in special registers Lo and Hi:  (Hi,Lo) = $t3 * $t4
			  (mult	$t3,$t4) */

    C_BEQ, // branch to target if  $t0 = $t1 (beq	$t0,$t1,target)
    C_BNE, // branch to target if  $t0 != $t1 (bne	$t0,$t1,target)
    C_BLE, // branch to target if  $t0 <= $t1 (ble	$t0,$t1,target)
    C_BGE, // branch to target if  $t0 >= $t1 (bge	$t0,$t1,target)
    C_BLT, // branch to target if  $t0 < $t1 (blt	$t0,$t1,target)
    C_BGT, // branch to target if  $t0 > $t1 (bgt	$t0,$t1,target)

    C_SEQ, /* Set register Rdest to 1 if register Rsrc1 equals Src2
			  and to be 0 otherwise.
			  (seq Rdest, Rsrc1, Src2) */

    C_SNE, /* Set register Rdest to 1 if register Rsrc1 is not equal
			  to Src2 and to 0 otherwise
			  (sne Rdest, Rsrc1, Src2) */
    C_SLE, /* Set register Rdest to 1 if register Rsrc1 is less than or
			  equal to Src2 and to 0 otherwise.
			  (sle Rdest, Rsrc1, Src2) */
    C_SGE, /* Set register Rdest to 1 if register Rsrc1 is greater than
			  or equal to Src2 and to 0 otherwise.
			  (sge Rdest, Rsrc1, Src2) */
    C_SLT, /* Set register Rdest to 1 if register Rsrc1 is less than
			  Src2 (or Imm) and to 0 otherwise.
			  (slt Rdest, Rsrc1, Src2) */
    C_SGT, /* Set register Rdest to 1 if register Rsrc1 is greater than
 			  Src2 and to 0 otherwise.
			  (sgt Rdest, Rsrc1, Src2) */

    C_LW, // Load word (lw $t, offset($s))
    C_LI, //load immediate value into destination register (li	register_destination, value)
    C_SW, // store word in source register into RAM destination (sw	register_source, RAM_destination)
    C_MOVE, // move $t2 = $t3 (move	$t2,$t3)
    C_J,
  
    C_SYSCALL,
    C_LABEL,
    C_SPACE

  } OpKind;



Address mkInt(int i);
Address mkVar(char *v);
#endif