.data
	newLine:.asciiz "\n"
.text
li $t0 0
sw $t0 0($gp)
j main
func:
move $fp $sp
addi $sp $sp -8
lw $t0 0($fp)
bgt $t0 10 lable_0
j lable_1
lable_0:
addi $sp $sp 8
jr $ra
j lable_2
lable_1:
lable_2:
lw $t0 0($gp)
addi $t2 $t0 1
sw $t2 -4($fp)
lw $t0 -4($fp)
sw $t0 0($gp)
addi $sp $sp 8
jr $ra
main:
move $fp $sp
addi $sp $sp 0
li $t0 1
li $a0 1
sw $t0 -8($sp)
addi $sp $sp -8
sw $ra 4($sp)
sw $fp 8($sp)
jal func
lw $ra 4($sp)
lw $fp 8($sp)
addi $sp $sp 8
lw $a0 0($gp)
li $v0 1
syscall
la $a0 newLine
li $v0 4
syscall
li $v0 10
syscall
