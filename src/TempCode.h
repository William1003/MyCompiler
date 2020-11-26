#ifndef _TEMP_CODE_H_
#define _TEMP_CODE_H_

#include<fstream>
#include<vector>
#include"Quaternion.h"
#include"SymbolTable.h"

using namespace std;

class TempCode
{
private:
	ofstream tempCode;
	string oper1;
	string oper2;
	string dest;
	SymbolTable& symbolTable;
public:
	TempCode(SymbolTable& symbolTable);
	void add(Quaternion quaternion);
	string genTempVar(string domain);
	string genLable();
	void setOper1(string oper1) 
	{
		this->oper1 = oper1;
	}
	void setOper2(string oper2)
	{
		this->oper2 = oper2;
	}
	void setDest(string dest)
	{
		this->dest = dest;
	}
	string getOper1()
	{
		return oper1;
	}
	string getOper2()
	{
		return oper2;
	}
	string getDest()
	{
		return dest;
	}
	void outputToFile();
	vector<Quaternion> getQuaternions()
	{
		return quaternions;
	}
	vector<Quaternion> quaternions;
	SymbolCode relationOp;
	void genBranch(string left, string right, string lable);
};

#endif // _TEMP_CODE_H_
