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

  #print the result
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
  li    $v0, 5
  syscall
  move  $t0,$v0
  # get M
  li    $v0, 5
  syscall
  move  $t1, $v0
  # Setup up arguments and call mm_alloc - v0 is returned as base address
  move  $a0, $t0
  move  $a1, $t1
  addi  $sp,$sp,-4
  sw    $ra,0($sp)
  jal mm_alloc
  #calls malloc
  move  $t5,$v0 
  la    $t6,0($v0)
  li    $t3,0
  # do nested loops to read in values
loop1:
  beq  $t0,$t3, exit
  li   $t4, 0
  addi $t3,$t3,1
  j loop2
loop2:
  beq  $t1,$t4, loop1
  li   $v0,5
  syscall
  sw   $v0,0($t5)
  addi $t5,$t5,4
  addi $t4,$t4,1
  j loop2
  # Note: third return value goes on the stack *after* restoration below
exit:
  move $v0,$t0
  move $v1,$t1
  # setup up return values
  lw   $ra, 0($sp)
  addi $sp,$sp,4
  sw   $t6,0($sp)
  # restore stack, ra, and any saved registers, if necessary
  jr   $ra
  # return to main

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
  # Allocate sufficient space=N*M*4
  li  $v0,9
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
  li  $t0,0
loop3:
  beq $t3,$t0, exit2
  li  $t1, 0
  li  $v0,4 
  j loop4
  #a loop that goes over the rows 

break3:
  addi $t0,$t0,1
  li   $v0,4 
  la   $a0,newline
  syscall
  j loop3
  #when the loop that goes over columns finishes incriment the cxounter and print a newline
loop4:
  beq  $a1,$t1, break3
  li   $v0,1
  lw   $a0,0($a2)
  syscall
  li   $v0,4
  la   $a0,tab
  syscall
  addi $t1,$t1,1
  addi $a2,$a2,4
  j loop4
  #loops the max number of columns and prints a row
exit2:

  # return to main
  jr  $ra

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
  lw   $t0,0($sp)
  addi $sp, $sp, -24
  sw   $s0,0($sp)
  sw   $s1,4($sp)
  sw   $s2,8($sp)
  sw   $s3,12($sp)
  sw   $s4,16($sp)
  sw   $ra 20($sp)
  move $s0,$a0
  move $s1,$a1
  move $s2,$a2
  move $s3,$a3
  move $a1,$a3
  # save return address and any saved registers on the stack, if necessary
  jal mm_alloc #calls mm_alloc
  move $t6,$v0 #saves a pointer to result matrix 
  move $t7,$v0 #makes a copy to return 
  # set up and call mm_alloc for result matrix
  li   $t1,0 #loads counter t0
  # three nested loops for matrix-matrix multiplication
loop5:
  beq  $t1,$s0,exit3
  li   $t2,0
  j loop6
#loop over the columns of matrix 1
break2:
  addi $t1, $t1, 1
  j loop5
#ensures addition to counter is done after completion of the task 
loop6:
  beq  $t2, $s3,break2
  li   $t3,0
  li   $t4,0
  j loop7
#loops over the columns of matrix 2 
break1:
  sw   $t4,0($t6)
  addi $t6, $t6, 4
  addi $t2, $t2,1
  j loop6
#ensures addition to counter is done after completion of the task 
loop7:
  beq  $t3, $s1, break1
  move $a0, $t1
  move $a1,$t3
  move $a2,$s2
  move $a3, $s1
  #sets up get value call
  jal get_value
  li   $t5,0
  #calls get value and stroes it in $t5
  move $t5,$v0 
  move $a0,$t3
  move $a1,$t2
  move $a2,$t0
  move $a3,$s3
  jal get_value
  #calls get value
  mul  $t5, $t5,$v0
  #multiplies the two values together 
  add  $t4,$t4,$t5
  #adds them to the total for that index 
  addi $t3,$t3,1
  j loop7
exit3:
  lw   $s0,0($sp)
  lw   $s1,4($sp)
  lw   $s2,8($sp)
  lw   $s3,12($sp)
  lw   $s4,16($sp)
  lw   $ra 20($sp)
  # restore stack, ra, and any saved registers, if necessary
  addi $sp, $sp, 24
  move $v0, $t7
  # set return value

  
  # return to main
  jr   $ra
################################################################################
# get_value: returns the value of the matrix at a given index
# Input:
#   a0 = N for matrix
#   a1 = M for matrix
#   a2 = Base pointer 
#   a3 = Max M
# Return:
#   v0 = val in matrix 
################################################################################
get_value:
  mul $a0,$a0,$a3
  #multiplies the number of rows by the max number of columns 
  add $a0, $a0, $a1
  #add the curent number of columns 
  sll $a0, $a0, 2
  #uses sll to multiply the index by 4
  add $a2,$a2,$a0
  #adds it to the array
  lw  $v0, 0($a2)
  #loads value 
  jr $ra
  