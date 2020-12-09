.data
	newLine:.asciiz "\n"
.text
addiu $t0 $gp 0
li $t2 1
sw $t2 0($t0)
addiu $t0 $gp 0
li $t2 2
sw $t2 4($t0)
addiu $t0 $gp 0
li $t2 3
sw $t2 8($t0)
addiu $t0 $gp 0
li $t2 4
sw $t2 12($t0)
addiu $t0 $gp 0
li $t2 5
sw $t2 16($t0)
addiu $t0 $gp 0
li $t2 6
sw $t2 20($t0)
addiu $t0 $gp 0
li $t2 7
sw $t2 24($t0)
addiu $t0 $gp 0
li $t2 8
sw $t2 28($t0)
addiu $t0 $gp 0
li $t2 9
sw $t2 32($t0)
addiu $t0 $gp 0
li $t2 10
sw $t2 36($t0)
j main
fun:
move $fp $sp
addi $sp $sp 0
addiu $sp $sp 0
li $v0 1
jr $ra
addiu $sp $sp 0
jr $ra
main:
move $fp $sp
addi $sp $sp -52
addiu $t0 $fp 0
li $t2 22
sw $t2 0($t0)
addiu $t0 $fp 0
li $t2 33
sw $t2 -4($t0)
addiu $t0 $fp 0
li $t2 44
sw $t2 -8($t0)
addiu $t0 $fp 0
li $t2 55
sw $t2 -12($t0)
addiu $t0 $fp 0
addiu $t0 $t0 -12
lw $t0 0($t0)
sw $t0 -16($fp)
lw $t0 -16($fp)
subiu $t2 $t0 52
sw $t2 -20($fp)
addiu $t0 $fp 0
lw $t1 -20($fp)
sll $t1 $t1 2
subu $t0 $t0 $t1
lw $t0 0($t0)
sw $t0 -24($fp)
li $t0 2
lw $t1 -24($fp)
mult $t0 $t1
mflo $t2
sw $t2 -28($fp)
addi $sp $sp -8
sw $ra 4($sp)
sw $fp 8($sp)
jal fun
lw $ra 4($sp)
lw $fp 8($sp)
addi $sp $sp 8
move $t0 $v0
sw $t0 -32($fp)
lw $t0 -32($fp)
li $t1 108
mult $t0 $t1
mflo $t2
sw $t2 -36($fp)
lw $t0 -28($fp)
lw $t1 -36($fp)
subu $t2 $t0 $t1
sw $t2 -40($fp)
addiu $t0 $fp 0
lw $t1 -40($fp)
li $t2 333
sll $t1 $t1 2
subu $t0 $t0 $t1
sw $t2 0($t0)
li $t0 10
li $t1 5
div $t0 $t1
mflo $t2
sw $t2 -44($fp)
addiu $t0 $fp 0
lw $t1 -44($fp)
sll $t1 $t1 2
subu $t0 $t0 $t1
lw $t0 0($t0)
sw $t0 -48($fp)
lw $a0 -48($fp)
li $v0 1
syscall
la $a0 newLine
li $v0 4
syscall
li $v0 10
syscall
