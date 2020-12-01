.data
	newLine:.asciiz "\n"
	string0:.asciiz "\\n"
.text
j main
main:
move $fp $sp
addi $sp $sp 0
la $a0 string0
li $v0 4
syscall
la $a0 newLine
li $v0 4
syscall
li $v0 10
syscall
