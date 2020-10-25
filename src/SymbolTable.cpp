#include "SymbolTable.h"

SymbolTable::SymbolTable()
{
	table.clear();
	count = 0;
	symbolTableFile.open("symbolTable.txt");
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

Function* SymbolTable::getFunction(string name)
{
	for (int i = 0; i < count; i++)
	{
		if (table[i].isFunction(name))
		{
			return dynamic_cast<Function*>(&table[i]);
		}
	}
	return nullptr;
}

void SymbolTable::findParameter(string functionName)
{
	for (int i = 0; i < count; i++)
	{
		if (table[i].nameEqual(functionName))
		{
			index = i + 1;
			break;
		}
	}
}

SymbolTableItemType SymbolTable::getParaType(string functionName)
{
	if (index >= count)
	{
		return VOID;
	}
	if (table[index].isParameter(functionName))
	{
		return table[index++].getType();
	}
	return VOID;
}

SymbolTableItemType SymbolTable::getCurrentType()
{
	return table[index].getType();
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