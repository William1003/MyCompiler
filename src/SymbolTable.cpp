#include "SymbolTable.h"

SymbolTable::SymbolTable()
{
	table.clear();
	count = 0;
}

bool SymbolTable::push(SymbolTableItem item)
{
	table.push_back(item);
	count++;
	return true;
}

bool SymbolTable::hasRet(string functionName)
{
	for (size_t i = 0; i < count; i++)
	{
		if (table[i].nameEqual(functionName))
		{
			return table[i].hasRet();
		}
	}
	return 0;
}