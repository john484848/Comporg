.data
	x: .word 4
	y: .word 7
	newline: .asciiz "\n"
.text
	main:
		lw $t0, x
		lw $t1, y
		slt $t3,$t1,$t0
		beq $t3, $zero, Stuff
		li $v0, 1
		move $a0,$t0
		syscall
		li $v0, 4
		la $a0, newline
		syscall
		li $v0, 10
		syscall
	Stuff:
		addi $t0, $t0,-4
		li $v0, 1
		move $a0,$t0
		syscall
		li $v0, 4
		la $a0, newline
		syscall
		li $v0, 10
		syscall
		
