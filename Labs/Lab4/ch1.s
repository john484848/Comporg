.data 
    even: .asciiz "EVEN\n"
    odd: .asciiz "ODD\n"
.text 
main:
    li $a0, 5
    jal Odd_or_Even
    li $v0, 10
    syscall
Odd_or_Even:
    addi $sp,$sp,-4
    sw $s0, 0($sp)
    li $s0, 2
    div $a0,$s0
    mfhi $t0
    move $v0,$t0
    lw $s0,0($sp)
    addi $sp,$sp,4
    beq	$t0, $zero, print_even	
    li $v0,4
    la $a0, odd
    syscall
    j $ra
print_even:
    li $v0,4
    la $a0, even
    syscall
    j $ra   