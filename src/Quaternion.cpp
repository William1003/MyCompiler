#include"Quaternion.h"

string Quaternion::toString()
{
	string s;
	switch (op)
	{
	case quaternion::ADD:
		s = dest + " = " + oper1 + "  +" + oper2;
		break;
	case quaternion::SUB:
		s = dest + "  =" + oper1 + " - " + oper2;
		break;
	case quaternion::MULT:
		s = dest + " = " + oper1 + " * " + oper2;
		break;
	case quaternion::DIV:
		s = dest + " = " + oper1 + " / " + oper2;
		break;
	case quaternion::CONSTINT:
		s = "const int " + oper1 + " = " + oper2;
		break;
	case quaternion::CONSTCHAR:
		s = "const char " + oper1 + " = " + oper2;
		break;
	case quaternion::VARINT:
		if (oper2.empty())
		{
			s = "var int " + oper1;
		}
		else
		{
			s = "var int " + oper1 + " = " + oper2;
		}
		break;
	case quaternion::VARCHAR:
		if (oper2.empty())
		{
			s = "var char " + oper1;
		}
		else
		{
			s = "var char " + oper1 + " = " + oper2;
		}
		break;
	case quaternion::READINT:
		s = "read int " + oper2;
		break;
	case quaternion::READCHAR:
		s = "read char " + oper2;
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
	default:
		break;
	}
	return s;
}
