#ifndef _SYMBOL_TABLE_ITEM_FACTORY_H_
#define _SYMBOL_TABLE_ITEM_FACTORY_H_

#include"SymbolTableItem.h"
#include"constValue.h"

class SymbolTableItemFactory
{
public:
	SymbolTableItemFactory();
	SymbolTableItem create(string name, string domain, SymbolTableItemType type, int value);
	SymbolTableItem create(string name, string domain, SymbolTableItemType type, char value);
};

#endif // !_SYMBOL_TABLE_ITEM_FACTORY_H_
