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