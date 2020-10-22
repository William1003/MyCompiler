#ifndef _SYMBOL_TABLE_H_
#define _SYMBOL_TABLE_H_

#include<vector>
#include"SymbolTableItem.h"
#include<map>

class SymbolTable
{
private:
	vector<SymbolTableItem> table;
	int count = 0;

public:
	SymbolTable();
	bool push(SymbolTableItem item);
	bool hasRet(string functionName);
};

#endif // !_SYMBOL_TABLE_H_
