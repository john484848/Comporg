################################################################################
# Homework 3: Matrix-matrix multiplication
# CSCI-2500 Fall 2021
################################################################################
## Data segment
################################################################################
.data
  newline:  .asciiz "\n"
  tab:      .asciiz "\t"

################################################################################
## Text segment
################################################################################
.text
  .align 2          # instructions must be on word boundaries
  .globl main       # main is a global label
  .globl mm_read    # read in NxM matrix
  .globl mm_alloc   # allocate space for NxM matrix
  .globl mm_print   # print out formatted NxM matrix
  .globl mm_mult    # multiple two non-square matrices

################################################################################
# Main segment
################################################################################
main:
  # save return address on the stack
  sub $sp, $sp, 4   
  sw  $ra, 0($sp)

  # Read in a matrix 1 of some NxM size
  # Return in $v0 - N
  # Return in $v1 - M
  # Return in 0($sp) - base address of matrix
  sub $sp, $sp, 4   # make space on stack for return value
  jal mm_read

  # save matrix 1 values
  move  $s0, $v0
  move  $s1, $v1
  lw    $s2, 0($sp)
  add   $sp, $sp, 4   # restore stack

  # Setup arguments and print out matrix 1
  move  $a0, $s0
  move  $a1, $s1
  move  $a2, $s2
  jal mm_print

  #Read in matrix 2 of some NxM size
  sub $sp, $sp, 4   # make space on stack for return value
  jal mm_read

  #save matrix 2 values
  move  $s3, $v0
  move  $s4, $v1
  lw    $s5, 0($sp)
  add   $sp, $sp, 4   # restore stack

  # Setup arguments and print out matrix 2
  move  $a0, $s3
  move  $a1, $s4
  move  $a2, $s5
  jal mm_print

  # Setup arguments and call mm_mult
  # Note: 5 arguments, so we need to use stack
  # Note: num cols matrix 1 == num rows matrix 2
  move  $a0, $s0    # num rows matrix1
  move  $a1, $s1    # num cols matrix1
  move  $a2, $s2    # address of matrix 1
  move  $a3, $s4    # num cols matrix2
  sub   $sp, $sp, 4   
  sw    $s5, 0($sp) # address of matrix 2
  jal mm_mult
  add   $sp, $sp, 4

  print the result
  move $a0, $s0 
  move $a1, $s4
  move $a2, $v0
  jal mm_print

  # restore $ra, free stack space, and return
  lw  $ra, 0($sp)
  add $sp, $sp, 4
  jr  $ra

################################################################################
# mm_read: Read in a NxM matrix from standard input
# Note: You will be calling mm_alloc from this function, so you'll need to save 
#       $ra on the stack. Also, you'll be returning 3 values back to main, one
#       value (the matrix base address), will need to be saved on the stack.
#       Matrix format is:
#         First line:  N = num rows
#         Second line: M = num cols
#         Subsequent lines contain one value each, filling each row in turn
#         (same format as hw01, except N,M on separate lines)  
# Input:
#   None
# Return:
#   v0 = N
#   v1 = M
#   0($sp) = base address of matrix
################################################################################
mm_read:
  # save return address and any saved registers on the stack, if necessary

  # get N
  li $v0, 5
  syscall
  move $t0,$v0
  # get M
  li $v0, 5
  syscall
  move $t1, $v0
  # Setup up arguments and call mm_alloc - v0 is returned as base address
  move $a0, $t0
  move $a1, $t1
  addi $sp,$sp,-4
  sw $ra, 0($sp)
  jal mm_alloc
  lw $ra, 0($sp)
  move $t5,$v0 
  la $t6,0($v0)
  li $t3, 0
  # do nested loops to read in values
loop1:
  beq $t0,$t3, exit
  li $t4, 0
  addi $t3,$t3,1
  j loop2
loop2:
  beq $t1,$t4, loop1
  li $v0,5
  syscall
  sw $v0,0($t5)
  addi $t5,$t5,4
  addi $t4,$t4,1
  j loop2
  # setup up return values
  # Note: third return value goes on the stack *after* restoration below
exit:
  #addi $t6, $t6,4
  #li $v0, 1
  #lw $a0,0($t6)
  #syscall
  move $v0,$t0
  move $v1,$t1
  # restore stack, ra, and any saved registers, if necessary
  addi $sp,$sp,4
  sw $t6,0($sp)
  # return to main
  jr  $ra

################################################################################
# mm_alloc: Allocate an NxM matrix
# Note: You can just allocate a single array of length N*M.
# Input:
#   a0 = N
#   a1 = M
# Return:
#   v0 = base address of allocated space
################################################################################
mm_alloc:
  # save return address and any saved registers on the stack, if necessary
  mul $a0,$a0,$a1
  sll $a0,$a0,2
  # Allocate sufficient space
  li $v0,9
  syscall
  # set return value

  # restore stack, ra, and any saved registers, if necessary

  # return to main
  jr  $ra

################################################################################
# mm_print: Output the formatted contents of NxM matrix
# Note: Within a row, separate each value by tab
#       End each row with a newline
# Input:
#   a0 = N
#   a1 = M
#   a2 = base pointer
# Return:
#   None
################################################################################
mm_print:
  # save return address and any saved registers on the stack, if necessary
  move $t3,$a0

  # do nested loops to print out values
  li $t0,0
  j loop4
loop3:
  beq $t3,$t0, exit2
  li $t1, 0
  addi $t0,$t0,1
  li $v0,4 
  la $a0,newline
  syscall
  j loop4
loop4:
  beq $a1,$t1, loop3
  li $v0,1
  lw $a0,0($a2)
  syscall
  li $v0,4
  la $a0,tab
  syscall
  addi $t1,$t1,1
  addi $a2,$a2,4
  j loop4
  # setup up return values
  # Note: third return value goes on the stack *after* restoration below
exit2:
  li $v0,4 
  la $a0,newline
  syscall
  # restore stack, ra, and any saved registers, if necessary

  # return to main
  #jr  $ra

################################################################################
# mm_mult: Perform the actual NxM matrix multiplication
# Input:
#   a0 = N for matrix 1
#   a1 = M for matrix 1 <-> N for matrix 2
#   a2 = base address for matrix 1
#   a3 = M for matrix 2
#   0($sp) = base address for matrix 2
# Return:
#   v0 = base address for result matrix
################################################################################
mm_mult:
  # save return address and any saved registers on the stack, if necessary
  addi $sp, $sp, -4
  move $a0,$t0
  move $a1,$t1

  # set up and call mm_alloc for result matrix
 
  # three nested loops for matrix-matrix multiplication

  # set return value

  # restore stack, ra, and any saved registers, if necessary

  # return to main
  jr  $ra
