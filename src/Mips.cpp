#include"Mips.h"

#define NEWLINE do {mips << "la $a0 newLine" << endl;\
					mips << "li $v0 4" << endl;\
					mips << "syscall" << endl;} while (0)

Mips::Mips(TempCode& tempCode, SymbolTable& symbolTable) : tempCode(tempCode), symbolTable(symbolTable)
{
	tempCodeFile.open("tempCode.txt");
	mips.open("mips.txt");
	
}

void Mips::generate()
{
	int strCount = 0;
	string currentDomain = "0";
	mips << ".data" << endl;
	mips << "\tnewLine:.asciiz \"\\n\"" << endl;
	// 扫描第一遍, 把字符串找出来
	for (int i = 0; i < tempCode.quaternions.size(); i++)
	{
		Quaternion q = tempCode.quaternions[i];
		if (q.op == quaternion::PRINTS || q.op == quaternion::PRINTSV)
		{
			mips << "\tstring" << to_string(strCount++) << ":.asciiz " << "\"" << q.oper1 << "\"" << endl;
		}
	}
	strCount = 0;
	mips << ".text" << endl;
	bool jMainFlag = false;
	for (int i = 0; i < tempCode.quaternions.size(); i++)
	{
		Quaternion q = tempCode.quaternions[i];
		switch (q.op)
		{
		case quaternion::ADD:
			break;
		case quaternion::SUB:
			break;
		case quaternion::MULT:
			break;
		case quaternion::DIV:
			break;
		case quaternion::CONSTINT:
			break;
		case quaternion::CONSTCHAR:
			break;
		case quaternion::VARINT:
		case quaternion::VARCHAR:
			if (!q.oper2.empty())
			{
				SymbolTableItem item = symbolTable.getItem(q.oper1, currentDomain);
				string domain = item.getDomain();
				if (domain == "0")
				{
					mips << "li $t0 " << q.oper2 << endl;
					mips << "sw $t0 " << to_string(item.addr * 4) << "($gp)" << endl;
				}
				else
				{

				}
			}
			break;
		case quaternion::READINT:
			break;
		case quaternion::READCHAR:
			break;
		case quaternion::PRINTS:
			mips << "la $a0 string" << to_string(strCount++) << endl;
			mips << "li $v0 4" << endl;
			mips << "syscall" << endl;
			NEWLINE;
			break;
		case quaternion::PRINTVAR:
			mips << "li $a0 " << q.oper1 << endl;
			if (q.dest == "char")
			{
				mips << "li $v0 11" << endl;
			}
			else
			{
				mips << "li $v0 1" << endl;
			}
			mips << "syscall" << endl;
			NEWLINE;
			break;
		case quaternion::PRINTSV:
			mips << "la $a0 string" << to_string(strCount++) << endl;
			mips << "li $v0 4" << endl;
			mips << "syscall" << endl;
			mips << "li $a0 " << q.oper2 << endl;
			if (q.dest == "char")
			{
				mips << "li $v0 11" << endl;
			}
			else
			{
				mips << "li $v0 1" << endl;
			}
			mips << "syscall" << endl;
			NEWLINE;
			break;
		case quaternion::ASSIGN:
			break;
		case quaternion::FUNCTIONDEF:
			if (!jMainFlag)
			{
				mips << "j main" << endl;
				jMainFlag = true;
			}
			mips << q.oper2 << ":" << endl;
			currentDomain = q.oper2;
			break;
		default:
			break;
		}
	}
	// 结束
	mips << "li $v0 10" << endl;
	mips << "syscall" << endl;
}

