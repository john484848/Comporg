.data 
	newline: .asciiz "\n"
.text
	main:
		li $v0, 5
		syscall
		move $t0, $v0
		li $v0, 5
		syscall
		slt $t3, $t1, $t0
		beq $t3, $zero, Swap
		li $v0, 
		jr $ra
		li $v0, 10
		syscall

	Swap:
		move $t4, $t0
		move $t5, $t1
		move $t0, $t5
		move $t1, $t4
		li $v0, 1
		move $a0, $t0
		syscall
		li $v0, 1
		move $a0, $t1
		syscall
