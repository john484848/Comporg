.data
list: .word 1, 2, 3, 4,5 
.text 
main:
    lw $a0,list
    li $a1, 4 
    jal Numodds
    move $s1,$v0
    li $v0,1
    syscall
    li $v0, 10
    syscall
Numodds:
    addi $sp, $sp, 12
    sw $ra, 0($sp)
    sw $t0, 4($sp)
    sw $s0, 8($sp)
    li $t0,0
    li $s0,0
loop1:
    beq	$t0, $a1, Exit	
    jal Odd_or_Even
    addi $t0,$t0,1
    add $s0,$s0,$v0
    addi $a0,$a0,4 
    j loop1  
Exit:
    move $v0,$s0
    lw $ra, 0($sp)
    lw $t0, 4($sp)
    lw $s0, 8($sp)
    addi $sp, $sp, 12
    j $ra
Odd_or_Even:
    addi $sp,$sp,-8
    sw $s0, 0($sp)
    sw $t0, 4($sp)
    li $s0, 2
    div $a0,$s0
    mfhi $t0
    move $v0,$t0
    lw $s0,0($sp)
    lw $t0,4($sp)
    addi $sp,$sp,8
    j $ra 

