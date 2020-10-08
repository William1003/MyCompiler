#include "SymbolTableItem.h"

SymbolTableItem::SymbolTableItem(string name, int functionType)
{
	this->name = name;
	this->functionType = functionType;
}

bool SymbolTableItem::hasName(string name)
{
	if (this->name == name)
	{
		return true;
	}
	return false;
}

bool SymbolTableItem::hasRet()
{
	return functionType > 0;
}
