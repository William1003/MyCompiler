.data
	newLine:.asciiz "\n"
	string0:.asciiz "array2[0]="
.text
move $t0 $gp 
addi $t0 $t0 0
li $t2 49
sw $t2 0($t0)
move $t0 $gp 
addi $t0 $t0 0
li $t2 50
sw $t2 4($t0)
move $t0 $gp 
addi $t0 $t0 0
li $t2 51
sw $t2 8($t0)
move $t0 $gp 
addi $t0 $t0 0
li $t2 52
sw $t2 12($t0)
move $t0 $gp 
addi $t0 $t0 16
li $t2 111
sw $t2 0($t0)
move $t0 $gp 
addi $t0 $t0 16
li $t2 22
sw $t2 4($t0)
move $t0 $gp 
addi $t0 $t0 16
li $t2 333
sw $t2 8($t0)
move $t0 $gp 
addi $t0 $t0 16
li $t2 444
sw $t2 12($t0)
li $t0 0
sw $t0 32($gp)
li $t0 1
sw $t0 36($gp)
j main
main:
move $fp $sp
addi $sp $sp -28
move $t0 $gp 
addi $t0 $t0 16
li $t2 1
sw $t2 8($t0)
li $t0 1
li $t1 2
mult $t0 $t1
mflo $t2
sw $t2 0($fp)
lw $t0 0($fp)
addi $t2 $t0 0
sw $t2 -4($fp)
move $t0 $gp 
addi $t0 $t0 0
lw $t1 -4($fp)
li $t2 57
li $t3 4
mult $t1 $t3
mflo $t3
add $t0 $t0 $t3
sw $t2 0($t0)
move $t0 $gp 
addi $t0 $t0 16
addi $t0 $t0 0
lw $t0 0($t0)
sw $t0 -8($fp)
la $a0 string0
li $v0 4
syscall
lw $a0 -8($fp)
li $v0 1
syscall
la $a0 newLine
li $v0 4
syscall
move $t0 $gp 
addi $t0 $t0 16
addi $t0 $t0 8
lw $t0 0($t0)
sw $t0 -12($fp)
lw $t0 -12($fp)
li $t1 2
mult $t0 $t1
mflo $t2
sw $t2 -16($fp)
lw $t0 -16($fp)
addi $t2 $t0 0
sw $t2 -20($fp)
move $t0 $gp 
addi $t0 $t0 0
lw $t1 -20($fp)
li $t2 4
mult $t1 $t2
mflo $t1
add $t0 $t0 $t1
lw $t0 0($t0)
sw $t0 -24($fp)
lw $a0 -24($fp)
li $v0 11
syscall
la $a0 newLine
li $v0 4
syscall
li $v0 10
syscall
