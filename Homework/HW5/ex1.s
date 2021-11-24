.data
.text
main:
#q = 12;
li $s0,12
#j = q - 2;
move $s1,$s0
addi $s1,$s1,-2
#x = q * q / j;
move $s2,$s0
mult $s2,$s0
mflo $s2
div $s2,$s1
mflo $s2
li $v0,1
move $a0,$s0
syscall
li $v0,1
move $a0,$s1
syscall
li $v0,1
move $a0,$s2
syscall
jr $ra