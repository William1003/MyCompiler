#ifndef _SYMBOL_TABLE_H_
#define _SYMBOL_TABLE_H_

#include<vector>
#include"SymbolTableItem.h"

class SymbolTable
{
private:
	vector<SymbolTableItem> table;
	int count;

public:
	SymbolTable();
	bool add(SymbolTableItem item);
	bool hasRet(string functionName);
};

#endif // !_SYMBOL_TABLE_H_
