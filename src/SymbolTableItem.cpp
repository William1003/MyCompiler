#include"SymbolTableItem.h"
#include"constValue.h"

bool SymbolTableItem::nameEqual(string name)
{
	if (this->name == name)
	{
		return true;
	}
	return false;
}

bool SymbolTableItem::hasRet()
{
	if (this->kind == FUNCTION)
	{
		return this->type > 0;
	}
	return false;
}
