.data
	newLine:.asciiz "\n"
.text
j main
func:
move $fp $sp
addi $sp $sp -8
li $t0 2
lw $t1 0($fp)
mult $t0 $t1
mflo $t2
sw $t2 -4($fp)
addi $sp $sp 8
lw $v0 -4($fp)
jr $ra
addi $sp $sp 8
jr $ra
add:
move $fp $sp
addi $sp $sp -12
lw $t0 0($fp)
lw $t1 -4($fp)
add $t2 $t0 $t1
sw $t2 -8($fp)
addi $sp $sp 12
lw $v0 -8($fp)
jr $ra
addi $sp $sp 12
jr $ra
main:
move $fp $sp
addi $sp $sp -8
li $t0 2
li $a1 2
sw $t0 -12($sp)
li $t0 1
li $a0 1
sw $t0 -8($sp)
addi $sp $sp -8
sw $ra 4($sp)
sw $fp 8($sp)
jal add
lw $ra 4($sp)
lw $fp 8($sp)
addi $sp $sp 8
move $t0 $v0
sw $t0 0($fp)
lw $t0 0($fp)
move $a0 $t0
sw $t0 -8($sp)
addi $sp $sp -8
sw $ra 4($sp)
sw $fp 8($sp)
jal func
lw $ra 4($sp)
lw $fp 8($sp)
addi $sp $sp 8
move $t0 $v0
sw $t0 -4($fp)
lw $a0 -4($fp)
li $v0 1
syscall
la $a0 newLine
li $v0 4
syscall
li $v0 10
syscall
