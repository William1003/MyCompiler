.data
	newLine:.asciiz "\n"
.text
j main
f:
move $fp $sp
addi $sp $sp -12
lw $t0 0($fp)
lw $t1 -4($fp)
add $t2 $t0 $t1
sw $t2 -8($fp)
addi $sp $sp 12
lw $v0 -8($fp)
jr $ra
main:
move $fp $sp
addi $sp $sp -8
li $t0 1
sw $t0 0($fp)
li $t0 1
sw $t0 -4($fp)
lw $t0 0($fp)
move $a0 $t0
sw $t0 -8($sp)
lw $t0 -4($fp)
move $a1 $t0
sw $t0 -12($sp)
addi $sp $sp -8
sw $ra 4($sp)
sw $fp 8($sp)
jal f
lw $ra 4($sp)
lw $fp 8($sp)
addi $sp $sp 8
move $a0 $v0
li $v0 1
syscall
la $a0 newLine
li $v0 4
syscall
li $v0 10
syscall
