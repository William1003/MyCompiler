#include"TempCode.h"

TempCode::TempCode(SymbolTable& symbolTable) : symbolTable(symbolTable)
{
	tempCode.open("tempCode.txt");
}

string TempCode::genTempVar(string domain)
{
	static int n = 0;
	string name = "#T" + to_string(n++);
	symbolTable.push(SymbolTableItem(name, domain, VAR, INT));
	return name;
}

void TempCode::outputToFile()
{
	for (int i = 0; i < quaternions.size(); i++)
	{
		tempCode << quaternions[i].toString() << endl;
	}
}

void TempCode::add(Quaternion quaternion)
{
	quaternions.push_back(quaternion);
}

string TempCode::genLable()
{
	static int count = 0;
	return "lable_" + to_string(count++);
}

void TempCode::genBranch(string left, string right, string lable)
{
	switch (relationOp)
	{
	case LSS:
		this->add(Quaternion(quaternion::LSS, left, right, lable));
		break;
	case LEQ:
		this->add(Quaternion(quaternion::LEQ, left, right, lable));
		break;
	case GRE:
		this->add(Quaternion(quaternion::GRE, left, right, lable));
		break;
	case GEQ:
		this->add(Quaternion(quaternion::GEQ, left, right, lable));
		break;
	case EQL:
		this->add(Quaternion(quaternion::EQL, left, right, lable));
		break;
	case NEQ:
		this->add(Quaternion(quaternion::NEQ, left, right, lable));
		break;
	default:
		break;
	}
}