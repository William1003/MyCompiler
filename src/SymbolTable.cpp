#include "SymbolTable.h"

SymbolTable::SymbolTable()
{
	table.clear();
	count = 1;
	symbolTableFile.open("symbolTable.txt");
	table.push_back(SymbolTableItem("0", "1", FUNCTION, VOID));
}

bool SymbolTable::hasItem(string name, string domain)
{
	if (domain != "0")
	{
		for (int i = count - 1; i > 0; i--)
		{
			if (table[i].isItemInDomain(name, domain))
			{
				index = i;
				return true;
			}
		}
		for (int i = 0; i < count; i++)
		{
			if (table[i].isItemInDomain(name, "0"))
			{
				index = i;
				return true;
			}
		}
	}
	else
	{
		for (int i = 0; i < count; i++)
		{
			if (table[i].isItemInDomain(name, "0"))
			{
				index = i;
				return true;
			}
		}
	}
	return false;
}

bool SymbolTable::push(const SymbolTableItem& item)
{
	for (int i = 0; i < count; i++)
	{
		if (table[i].itemDuplicated(item))
		{
			return false;
		}
	}
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

void SymbolTable::output()
{
	for (int i = 0; i < count; i++)
	{
		symbolTableFile << table[i].getAttr() << endl;
	}
}

SymbolTableItem& SymbolTable::getCurrent()
{
	if (index >= count)
	{
		return table[0];
	}
	return table[index++];
}

SymbolTableItemType SymbolTable::getCurrentType()
{
	if (index >= count)
	{
		return VOID;
	}
	return table[index++].getType();
}

SymbolTableItemType SymbolTable::getFunctionType(string functionName)
{
	for (int i = 0; i < count; i++)
	{
		if (table[i].isItemInDomain(functionName, "0"))
		{
			return table[i].getType();
		}
	}
	return VOID;
}

SymbolTableItemKind SymbolTable::getCurrentKind()
{
	return table[index].getKind();
}

bool SymbolTable::hasFunction(string functionName)
{
	for (int i = 0; i < count; i++)
	{
		if (table[i].isFunction(functionName))
		{
			index = i;
			return true;
		}
	}
	index = 0;
	return false;
}