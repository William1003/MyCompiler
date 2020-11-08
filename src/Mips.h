#ifndef _MIPS_H_
#define _MIPS_H_

#include<fstream>
#include<vector>
#include"TempCode.h"
#include"SymbolTable.h"
#include"Quaternion.h"

using namespace std;

class Mips
{
private:
	ifstream tempCodeFile;
	ofstream mips;
	TempCode& tempCode;
	SymbolTable& symbolTable;

public:
	Mips(TempCode& tempCode, SymbolTable& symbolTable);
	void generate();
};
#endif // !_MIPS_H_
