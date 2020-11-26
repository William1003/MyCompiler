#include"Mips.h"
#include <iostream>

#define NEWLINE do {mips << "la $a0 newLine" << endl;\
					mips << "li $v0 4" << endl;\
					mips << "syscall" << endl;} while (0)

Mips::Mips(TempCode& tempCode, SymbolTable& symbolTable) : tempCode(tempCode), symbolTable(symbolTable)
{
	tempCodeFile.open("tempCode.txt");
	mips.open("mips.txt");
}

int Mips::findFreeTReg()
{
	for (int i = 0; i < REG_NUM; i++)
	{
		if (tregs[i] == 0)
		{
			tregs[i] = 1;
			return i;
		}
	}
	return -1;
}

void Mips::freeReg(int i)
{
	tregs[i] = 0;
}

void Mips::loadValue(string regName, string name, string domain, bool loadConst, string& value, bool& inReg)
{
	// 是符号
	if (name == "_functionRet")
	{
		mips << "move " << regName << " $v0" << endl;
		inReg = true;
		return;
	}
	if (symbolTable.hasItem(name, domain))
	{
		SymbolTableItem& item = symbolTable.getCurrent();
		// 常量
		if (item.getKind() == CONST)
		{
			value = (item.getType() == INT) ? to_string(item.getIntValue()) : to_string(item.getCharValue());
			if (loadConst)
			{
				mips << "li " << regName << " " << value << endl;
				inReg = true;
			}
		}
		// 变量
		else
		{
			// 局部变量
			if (item.getDomain() != "0")
			{
				mips << "lw " << regName << " " << to_string(-item.addr * 4) << "($fp)" << endl;
			}
			// 全局变量
			else
			{
				mips << "lw " << regName << " " << to_string(item.addr * 4) << "($gp)" << endl;
			}
			inReg = true;
		}
	}
	// 是常量
	else
	{
		if (loadConst)
		{
			mips << "li " << regName << " " << name << endl;
			inReg = true;
		}
		value = name;
	}
}

void Mips::saveValue(string name, string domain, string regName)
{
	SymbolTableItem item = symbolTable.getItem(name, domain);
	if (item.getDomain() == "0")
	{
		mips << "sw " << regName << " " << to_string(item.addr * 4) << "($gp)" << endl;
	}
	else
	{
		mips << "sw " << regName << " " << to_string(-item.addr * 4) << "($fp)" << endl;
	}
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
		{
			bool inReg1 = false;
			bool inReg2 = false;
			string value1, value2;
			loadValue("$t0", q.oper1, currentDomain, false, value1, inReg1);
			loadValue("$t1", q.oper2, currentDomain, false, value2, inReg2);
			if (inReg1 && inReg2)
			{
				mips << "add $t2 $t0 $t1" << endl;
			}
			else if(inReg1 && !inReg2)
			{
				mips << "addi $t2 $t0 " << value2 << endl;
			}
			else if (!inReg1 && inReg2)
			{
				mips << "addi $t2 $t1 " << value1 << endl;;
			}
			else
			{
				int v1 = atoi(value1.c_str());
				int v2 = atoi(value2.c_str());
				mips << "li $t2 " << to_string(v1 + v2) << endl;
			}
			saveValue(q.dest, currentDomain, "$t2");
			break;
		}
		case quaternion::SUB:
		{
			bool inReg1 = false;
			bool inReg2 = false;
			string value1, value2;
			loadValue("$t0", q.oper1, currentDomain, false, value1, inReg1);
			loadValue("$t1", q.oper2, currentDomain, false, value2, inReg2);
			if (q.oper1.empty())
			{
				if (inReg2)
				{
					mips << "sub $t2 $0 $t1" << endl;
				}
				else
				{
					int v2 = -atoi(value2.c_str());
					mips << "addi $t2 $0 " << to_string(v2) << endl;
				}
			}
			else
			{
				if (inReg1 && inReg2)
				{
					mips << "sub $t2 $t0 $t1" << endl;
				}
				else if (inReg1 && !inReg2)
				{
					mips << "subi $t2 $t0 " << value2 << endl;;
				}
				else if (!inReg1 && inReg2)
				{
					mips << "subi $t2 $t1 " << value1 << endl;;
				}
				else
				{
					int v1 = atoi(value1.c_str());
					int v2 = atoi(value2.c_str());
					mips << "li $t2 " << to_string(v1 - v2) << endl;
				}
			}
			saveValue(q.dest, currentDomain, "$t2");
			break;
		}
		case quaternion::MULT:
		{
			bool inReg1 = false;
			bool inReg2 = false;
			string value1, value2;
			loadValue("$t0", q.oper1, currentDomain, true, value1, inReg1);
			loadValue("$t1", q.oper2, currentDomain, true, value2, inReg2);
			mips << "mult $t0 $t1" << endl;
			mips << "mflo $t2" << endl;
			saveValue(q.dest, currentDomain, "$t2");
			break;
		}
		case quaternion::DIV:
		{
			bool inReg1 = false;
			bool inReg2 = false;
			string value1, value2;
			loadValue("$t0", q.oper1, currentDomain, true, value1, inReg1);
			loadValue("$t1", q.oper2, currentDomain, true, value2, inReg2);
			mips << "div $t0 $t1" << endl;
			mips << "mflo $t2" << endl;
			saveValue(q.dest, currentDomain, "$t2");
			break;
		}
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
					mips << "li $t0 " << q.oper2 << endl;
					mips << "sw $t0 " << to_string(-item.addr * 4) << "($fp)" << endl;
				}
			}
			break;
		case quaternion::READINT:
		case quaternion::READCHAR:
		{
			if (q.op == quaternion::READINT)
			{
				mips << "li $v0 5" << endl;
			}
			else
			{
				mips << "li $v0 12" << endl;
			}
			mips << "syscall" << endl;
			SymbolTableItem item = symbolTable.getItem(q.oper2, currentDomain);
			if (item.getDomain() == "0")
			{
				mips << "sw $v0 " << to_string(item.addr * 4) << "($gp)" << endl;
			}
			else
			{
				mips << "sw $v0 " << to_string(-item.addr * 4) << "($fp)" << endl;
			}
			if (q.op == quaternion::READCHAR)
			{
				mips << "li $v0 12" << endl;
				mips << "syscall" << endl;
			}
			break;
		}
		case quaternion::PRINTS:
			mips << "la $a0 string" << to_string(strCount++) << endl;
			mips << "li $v0 4" << endl;
			mips << "syscall" << endl;
			NEWLINE;
			break;
		case quaternion::PRINTVAR:
			if (q.oper1 == "_funcRet")
			{
				mips << "move $a0 $v0" << endl;
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
			}
			if (!symbolTable.hasItem(q.oper1, currentDomain))
			{
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
			}
			else
			{
				SymbolTableItem item = symbolTable.getCurrent();
				if (item.getKind() == CONST)
				{
					string value = (item.getType() == INT) ? to_string(item.getIntValue()) : to_string(item.getCharValue());
					mips << "li $a0 " << value << endl;
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
				}
				else
				{
					if (item.getDomain() != "0")
					{
						mips << "lw $a0 " << to_string(-item.addr * 4) << "($fp)" << endl;
					}
					else
					{
						mips << "lw $a0 " << to_string(item.addr * 4) << "($gp)" << endl;
					}
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
				}
			}
			break;
		case quaternion::PRINTSV:
		{
			mips << "la $a0 string" << to_string(strCount++) << endl;
			mips << "li $v0 4" << endl;
			mips << "syscall" << endl;
			if (q.oper1 == "_funcRet")
			{
				mips << "move $a0 $v0" << endl;
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
			}
			if (!symbolTable.hasItem(q.oper2, currentDomain))
			{
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
			}
			else
			{
				SymbolTableItem item = symbolTable.getCurrent();
				if (item.getKind() == CONST)
				{
					string value = (item.getType() == INT) ? to_string(item.getIntValue()) : to_string(item.getCharValue());
					mips << "li $a0 " << value << endl;
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
				}
				else
				{
					if (item.getDomain() != "0")
					{
						mips << "lw $a0 " << to_string(-item.addr * 4) << "($fp)" << endl;
					}
					else
					{
						mips << "lw $a0 " << to_string(item.addr * 4) << "($gp)" << endl;
					}
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
				}
			}
			break;
		}
		case quaternion::ASSIGN:
		{
			string value;
			bool inReg = false;
			loadValue("$t0", q.oper2, currentDomain, true, value, inReg);
			saveValue(q.oper1, currentDomain, "$t0");
			break;
		}
		case quaternion::FUNCTION:
		{
			currentDomain = q.oper2;
			int varCount = symbolTable.addrCount[currentDomain];
			if (!jMainFlag)
			{
				mips << "j main" << endl;
				jMainFlag = true;
			}
			mips << q.oper2 << ":" << endl;
			mips << "move $fp $sp" << endl;
			mips << "addi $sp $sp " << to_string(-varCount * 4) << endl;
			break;
		}
		case quaternion::ARRAYASSIGN:
		{
			SymbolTableItem item = symbolTable.getItem(q.dest, currentDomain);
			if (item.getDomain() == "0")
			{
				mips << "move $t0 $gp " << endl;
				mips << "addi $t0 $t0 " << to_string(item.addr * 4) << endl;
			}
			else
			{
				mips << "move $t0 $sp" << endl;
				mips << "addi $t0 $t0 " << to_string(-item.addr * 4) << endl;
			}
			string value1, value2;
			bool inReg1 = false, inReg2 = false;
			// 下标
			loadValue("$t1", q.oper1, currentDomain, false, value1, inReg1);
			// 要存的值
			loadValue("$t2", q.oper2, currentDomain, true, value2, inReg2);
			int v1 = atoi(value1.c_str());
			if (inReg1)
			{
				mips << "li $t3 4" << endl;
				mips << "mult $t1 $t3" << endl;
				mips << "mflo $t3" << endl;
				if (item.getDomain() == "0")
				{
					mips << "add $t0 $t0 $t3" << endl;
				}
				else
				{
					mips << "sub $t0 $t0 $t3" << endl;
				}
				mips << "sw $t2 0($t0)" << endl;
			}
			else
			{
				if (item.getDomain() == "0")
				{
					mips << "sw $t2 " << to_string(v1 * 4) << "($t0)" << endl;
				}
				else
				{
					mips << "sw $t2 " << to_string(-v1 * 4) << "($t0)" <<endl;
				}
			}
			break;
		}
		case quaternion::GETARRAY:
		{
			SymbolTableItem item = symbolTable.getItem(q.oper1, currentDomain);

			if (item.getDomain() == "0")
			{
				mips << "move $t0 $gp " << endl;
				mips << "addi $t0 $t0 " << to_string(item.addr * 4) << endl;
			}
			else
			{
				mips << "move $t0 $sp" << endl;
				mips << "addi $t0 $t0 " << to_string(-item.addr * 4) << endl;
			}
			string value;
			bool inReg = false;
			loadValue("$t1", q.oper2, currentDomain, false, value, inReg);
			int v = atoi(value.c_str());
			if (inReg)
			{
				mips << "li $t2 4" << endl;
				mips << "mult $t1 $t2" << endl;
				mips << "mflo $t1" << endl;
				if (item.getDomain() == "0")
				{
					mips << "add $t0 $t0 $t1" << endl;
				}
				else
				{
					mips << "sub $t0 $t0 $t1" << endl;
				}
				mips << "lw $t0 0($t0)" << endl;
				saveValue(q.dest, currentDomain, "$t0");
			}
			else
			{
				if (item.getDomain() == "0")
				{
					mips << "addi $t0 $t0 " << to_string(v * 4) << endl;
				}
				else
				{
					mips << "addi $t0 $t0 " << to_string(-v * 4) << endl;
				}
				mips << "lw $t0 0($t0)" << endl;
				saveValue(q.dest, currentDomain, "$t0");
			}
		}
		case quaternion::LABLE:
		{
			mips << q.dest << ":" << endl;
			break;
		}
		case quaternion::JUMP:
		{
			mips << "j " << q.dest << endl;
			break;
		}
		case quaternion::LSS:
		case quaternion::LEQ:
		case quaternion::GRE:
		case quaternion::GEQ:
		case quaternion::EQL:
		case quaternion::NEQ:
		{
			string value1, value2;
			bool inReg1 = false, inReg2 = false;
			loadValue("$t0", q.oper1, currentDomain, true, value1, inReg1);
			loadValue("$t1", q.oper2, currentDomain, false, value2, inReg2);
			switch (q.op)
			{
			case quaternion::LSS:
			{
				if (inReg2)
				{
					mips << "blt $t0 $t1 " << q.dest << endl;
				}
				else
				{
					mips << "blt $t0 " << value2 << " " << q.dest << endl;
				}
				break;
			}
			case quaternion::LEQ:
			{
				if (inReg2)
				{
					mips << "ble $t0 $t1 " << q.dest << endl;
				}
				else
				{
					mips << "ble $t0 " << value2 << " " << q.dest << endl;
				}
				break;
			}
			case quaternion::GRE:
			{
				if (inReg2)
				{
					mips << "bgt $t0 $t1 " << q.dest << endl;
				}
				else
				{
					mips << "bgt $t0 " << value2 << " " << q.dest << endl;
				}
				break;
			}
			case quaternion::GEQ:
			{
				if (inReg2)
				{
					mips << "bge $t0 $t1 " << q.dest << endl;
				}
				else
				{
					mips << "bge $t0 " << value2 << " " << q.dest << endl;
				}
				break;
			}
			case quaternion::EQL:
			{
				if (inReg2)
				{
					mips << "beq $t0 $t1 " << q.dest << endl;
				}
				else
				{
					mips << "beq $t0 " << value2 << " " << q.dest << endl;
				}
				break;
			}
			case quaternion::NEQ:
			{
				if (inReg2)
				{
					mips << "bne $t0 $t1 " << q.dest << endl;
				}
				else
				{
					mips << "bne $t0 " << value2 << " " << q.dest << endl;
				}
				break;
			}
			default:
				break;
			}
		}
		case quaternion::PUSHPARA:
		{
			int paraNum = atoi(q.oper1.c_str());
			bool inReg = false;
			string value;
			loadValue("$t0", q.oper2, currentDomain, false, value, inReg);
			if (paraNum > 3)
			{
				if (!inReg)
				{
					mips << "li $t0 " << value << endl;
				}
				mips << "sw $t0 " << to_string(-4 * paraNum - 8) << "($sp)" << endl;
			}
			else
			{
				if (inReg)
				{
					mips << "move $a" << to_string(paraNum) << " $t0" << endl;
				}
				else
				{
					mips << "li $t0 " << value << endl;
					mips << "li $a" << to_string(paraNum) << " " << value << endl;
				}
				mips << "sw $t0 " << to_string(-4 * paraNum - 8) << "($sp)" << endl;
			}
			break;
		}
		case quaternion::CALLFUNCTION:
		{
			mips << "addi $sp $sp -8" << endl;
			mips << "sw $ra 4($sp)" << endl;
			mips << "sw $fp 8($sp)" << endl;

			mips << "jal " << q.dest << endl;
			
			mips << "lw $ra 4($sp)" << endl;
			mips << "lw $fp 8($sp)" << endl;
			mips << "addi $sp $sp 8" << endl;

			break;
		}
		case quaternion::RETURN:
		{
			int varCount = symbolTable.addrCount[currentDomain];
			mips << "addi $sp $sp " << to_string(varCount * 4) << endl;
			string value;
			bool inReg = false;
			if (q.dest != "")
			{
				loadValue("$v0", q.dest, currentDomain, true, value, inReg);
			}

			mips << "jr $ra" << endl;
			break;
		}
		default:
			break;
		}
	}
	// 结束
	mips << "li $v0 10" << endl;
	mips << "syscall" << endl;
}

