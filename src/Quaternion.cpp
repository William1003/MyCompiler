#include"Quaternion.h"

string Quaternion::toString()
{
	string s;
	switch (op)
	{
	case quaternion::ADD:
		s = dest + " = " + oper1 + " + " + oper2;
		break;
	case quaternion::SUB:
		s = dest + " = " + oper1 + " - " + oper2;
		break;
	case quaternion::MULT:
		s = dest + " = " + oper1 + " * " + oper2;
		break;
	case quaternion::DIV:
		s = dest + " = " + oper1 + " / " + oper2;
		break;
	case quaternion::CONSTINT:
		s = "constint " + oper1 + " = " + oper2;
		break;
	case quaternion::CONSTCHAR:
		s = "constchar " + oper1 + " = " + oper2;
		break;
	case quaternion::VARINT:
		if (oper2.empty())
		{
			s = "varint " + oper1;
		}
		else
		{
			s = "varint " + oper1 + " = " + oper2;
		}
		break;
	case quaternion::VARCHAR:
		if (oper2.empty())
		{
			s = "varchar " + oper1;
		}
		else
		{
			s = "varchar " + oper1 + " = " + oper2;
		}
		break;
	case quaternion::READINT:
		s = "readint " + oper2;
		break;
	case quaternion::READCHAR:
		s = "readchar " + oper2;
		break;
	case quaternion::PRINTS:
		s = "prints " + oper1;
		break;
	case quaternion::PRINTVAR:
		s = "printvar " + oper1;
 		break;
	case quaternion::PRINTSV:
		s = "printsv " + oper1 + " " + oper2;
		break;
	case quaternion::ASSIGN:
		s = "assign " + oper1 + " = " + oper2;
		break;
	case quaternion::FUNCTION:
		s = "function " + oper1 + " " + oper2;
		break;
	case quaternion::ARRAY1INT:
		s = "array1int " + oper1;
		break;
	case quaternion::ARRAY1CHAR:
		s = "array1char " + oper1;
		break;
	case quaternion::ARRAY2INT:
		s = "array2int " + oper1;
		break;
	case quaternion::ARRAY2CHAR:
		s = "array2char " + oper1;
		break;
	case quaternion::ARRAYASSIGN:
		s = "arrayassign " + dest + "[" + oper1 + "] = " + oper2;
		break;
	case quaternion::GETARRAY:
		s = "getarray " + dest + " = " + oper1 + "[" + oper2 + "]";
		break;
	default:
		break;
	}
	return s;
}
