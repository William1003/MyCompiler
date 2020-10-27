#include"SyntaxAnalysis.h"
#include<iostream>

using namespace std;

SyntaxAnalysis::SyntaxAnalysis(ErrorHandler& errorHandler, LexicalAnalysis& lexicalAnalysis, Output& output, SymbolTable& table)
	:errorHandler(errorHandler), lexicalAnalysis(lexicalAnalysis), output(output), symbolTable(table)
{
	symbolCode = IDENFR;
	currentValue = 0;
	currentIdentifier = "";
	currentDomain = "0";
}

void SyntaxAnalysis::getNext()
{
	int line = lexicalAnalysis.getLineCount();
	hasNext = lexicalAnalysis.getNextSymbol();
	symbolCode = lexicalAnalysis.getSymbolCode();
	if (line != lexicalAnalysis.getLineCount())
	{
		errorLine = line;
	}
	else
	{
		errorLine = lexicalAnalysis.getLineCount();
	}
}

void SyntaxAnalysis::backup(int line)
{
	lexicalAnalysis.backup();
	output.backup(line);
}

void SyntaxAnalysis::retract(int line)
{
	lexicalAnalysis.restore();
	symbolCode = lexicalAnalysis.getSymbolCode();
	output.retract(line);
}

void SyntaxAnalysis::doLexicalAnalysis()
{
	while (lexicalAnalysis.getNextSymbol());
}

void SyntaxAnalysis::startSyntaxAnalysis()
{
	getNext();
	if (!program())
	{
		cout << "NOT A PROGRAM!" << endl;
	}
	else
	{
		cout << "CONGRATULATIONS!" << endl << "This is a program!" << endl;
	}
}

/*＜程序＞ ::= ［＜常量说明＞］［＜变量说明＞］{ ＜有返回值函数定义＞ | ＜无返回值函数定义＞ }＜主函数＞*/
bool SyntaxAnalysis::program()
{
	if (!hasNext)
	{
		return false;
	}
	constDeclaration();
	varDeclaration();

	//无返回值函数与主函数的判断
	while (true)
	{
		currentDomain = "0";
		if (!hasNext)
		{
			break;
		}

		backup(67);

		if (symbolCode == VOIDTK)
		{
			getNext();
			if (hasNext && symbolCode == MAINTK)
			{
				retract(80);
				break;
			}
			retract(83);
			if (!functionWithoutRet())
			{
				return false;
			}
			
		}
		else if (!functionWithRet())
		{
			break;
		}
	}
	if (!mainFunction())
	{
		return false;
	}

	output.syntaxAnalysisOutput("程序");
	return true;
}

/*＜主函数＞ ::= void main‘(’‘)’ ‘{’＜复合语句＞‘}’ */
bool SyntaxAnalysis::mainFunction()
{
	if (!hasNext || symbolCode != VOIDTK)
	{
		return false;
	}

	getNext();
	if (!hasNext || symbolCode != MAINTK)
	{
		return false;
	}

	currentDomain = "main";
	currentIdentifier = "main";

	getNext();
	if (!hasNext || symbolCode != LPARENT)
	{
		return false;
	}

	getNext();
	if (!hasNext || symbolCode != RPARENT)
	{
		errorHandler.error(errorLine, L);
		lexicalAnalysis.setAutoComplete();
	}

	getNext();
	if (!hasNext || symbolCode != LBRACE)
	{
		return false;
	}

	getNext();

	if (!compoundStatement())
	{
		return false;
	}

	if (symbolCode != RBRACE)
	{
		return false;
	}

	getNext();
	if (hasNext)
	{
		return false;
	}

	output.syntaxAnalysisOutput("主函数");
	return true;
}

/* ＜常量说明＞ ::= const＜常量定义＞; { const＜常量定义＞; } */
bool SyntaxAnalysis::constDeclaration()
{
	if (!hasNext || symbolCode != CONSTTK)
	{
		return false;
	}

	getNext();
	if (!hasNext)
	{
		errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
		return false;
	}

	if (!constDefinition())
	{
		errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
		return false;
	}

	// 未分析结束, 文件已结束
	if (!hasNext)
	{
		errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
		return false;
	}
	// 缺失分号, 自动补全, 继续分析
	if (symbolCode != SEMICN)
	{
		errorHandler.error(errorLine, K);
		lexicalAnalysis.setAutoComplete();
	}

	getNext();
	
	while (true)
	{
		
		//如果已经读到结尾 || 下一个符号不是CONST, 常量说明分析结束
		if (!hasNext || symbolCode != CONSTTK)
		{
			break;
		}

		getNext();
		if (!constDefinition())
		{
			errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
			return false;
		}
		if (!hasNext)
		{
			errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
			return false;
		}
		if (symbolCode != SEMICN)
		{
			errorHandler.error(errorLine, K);
			lexicalAnalysis.setAutoComplete();
		}

		getNext();
	}
	output.syntaxAnalysisOutput("常量说明");
	return true;
}

/* ＜常量定义＞   ::=   int＜标识符＞＝＜整数＞{,＜标识符＞＝＜整数＞}  
   | char＜标识符＞＝＜字符＞{ ,＜标识符＞＝＜字符＞ } */
bool SyntaxAnalysis::constDefinition()
{
	if (!hasNext || (symbolCode != INTTK && symbolCode != CHARTK))
	{
		return false;
	}

	if (symbolCode == INTTK)
	{
		getNext();
		if (symbolCode != IDENFR)
		{
			errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
			return false;
		}
		
		currentIdentifier = lexicalAnalysis.getToken();

		getNext();
		if (symbolCode != ASSIGN)
		{
			errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
			return false;
		}

		getNext();

		if (!integer())
		{
			errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
			return false;
		}

		//填符号表
		if (!symbolTable.push(SymbolTableItem(currentIdentifier, currentDomain, CONST, currentValue)))
		{
			errorHandler.error(lexicalAnalysis.getLineCount(), B);
		}

		while (true)
		{
			if (!hasNext || symbolCode != COMMA)
			{
				break;
			}

			getNext();
			if (!hasNext)
			{
				errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
				return false;
			}

			if (symbolCode != IDENFR)
			{
				errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
				return false;
			}

			currentIdentifier = lexicalAnalysis.getToken();

			getNext();
			if (!hasNext)
			{
				errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
				return false;
			}

			if (symbolCode != ASSIGN)
			{
				errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
				return false;
			}

			getNext();
			if (!integer())
			{
				errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
				return false;
			}
			
			if (!symbolTable.push(SymbolTableItem(currentIdentifier, currentDomain, CONST, currentValue)))
			{
				errorHandler.error(lexicalAnalysis.getLineCount(), B);
			}
		}
	}

	else if (symbolCode == CHARTK)
	{
		getNext();
		if (symbolCode != IDENFR)
		{
			errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
			return false;
		}

		currentIdentifier = lexicalAnalysis.getToken();

		getNext();
		if (symbolCode != ASSIGN)
		{
			errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
			return false;
		}

		getNext();

		if (!hasNext)
		{
			errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
			return false;
		}

		if (symbolCode != CHARCON)
		{
			errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
			return false;
		}

		currentChar = lexicalAnalysis.getToken().c_str()[0];
		if (!symbolTable.push(SymbolTableItem(currentIdentifier, currentDomain, CONST, currentChar)))
		{
			errorHandler.error(lexicalAnalysis.getLineCount(), B);
		}

		getNext();

		while (true)
		{
			if (!hasNext || symbolCode != COMMA)
			{
				break;
			}

			getNext();
			if (!hasNext)
			{
				errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
				return false;
			}

			if (symbolCode != IDENFR)
			{
				errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
				return false;
			}

			currentIdentifier = lexicalAnalysis.getToken();

			getNext();
			if (!hasNext)
			{
				errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
				return false;
			}
			if (symbolCode != ASSIGN)
			{
				errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
				return false;
			}

			getNext();
			if (symbolCode != CHARCON)
			{
				errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
				return false;
			}

			currentChar = lexicalAnalysis.getToken().c_str()[0];
			if (!symbolTable.push(SymbolTableItem(currentIdentifier, currentDomain, CONST, currentChar)))
			{
				errorHandler.error(lexicalAnalysis.getLineCount(), B);
			}

			getNext();
		}
	}
	output.syntaxAnalysisOutput("常量定义");
	return true;
}

/*＜变量说明＞ ::= ＜变量定义＞;{＜变量定义＞;} */
bool SyntaxAnalysis::varDeclaration()
{
	if (!hasNext)
	{
		return false;
	}

	backup(380);

	if (currentDomain == "0")
	{
		if (symbolCode == CHARTK || symbolCode == INTTK)
		{
			getNext();
			if (hasNext && symbolCode == IDENFR)
			{
				getNext();
				if (hasNext && symbolCode == LPARENT)
				{
					retract(392);
					return false;
				}
			}
		}
		retract(397);
	}

	if (!varDefinition())
	{
		return false;
	}

	if (symbolCode != SEMICN)
	{
		errorHandler.error(errorLine, K);
		lexicalAnalysis.setAutoComplete();
	}

	getNext();

	while (true)
	{
		backup(415);

		if (currentDomain == "0")
		{
			if (symbolCode == CHARTK || symbolCode == INTTK)
			{
				getNext();
				if (hasNext && symbolCode == IDENFR)
				{
					getNext();
					if (hasNext && symbolCode == LPARENT)
					{
						retract(427);
						break;
					}
				}
			}
			retract(432);
		}

		if (!hasNext || !varDefinition())
		{
			break;
		}

		if (symbolCode != SEMICN)
		{
			errorHandler.error(errorLine, K);
			lexicalAnalysis.setAutoComplete();
		}

		getNext();

	}
	output.syntaxAnalysisOutput("变量说明");
	return true;
}

/*＜变量定义＞ ::= ＜变量定义无初始化＞|＜变量定义及初始化＞ */
bool SyntaxAnalysis::varDefinition()
{
	backup(456);
	if (!varWithInit())
	{
		retract(459);
		if (!varWithoutInit())
		{
			return false;
		}
	}
	output.syntaxAnalysisOutput("变量定义");
	return true;
}

/*＜变量定义无初始化＞ ::= ＜类型标识符＞(＜标识符＞|＜标识符＞'['＜无符号整数＞']'|
	＜标识符＞'['＜无符号整数＞']''['＜无符号整数＞']'){,(＜标识符＞|＜标识符＞'['＜无符号整数＞']'|
	＜标识符＞'['＜无符号整数＞']''['＜无符号整数＞']' )}*/
bool SyntaxAnalysis::varWithoutInit()
{
	if (!hasNext || (symbolCode != INTTK && symbolCode != CHARTK))
	{
		return false;
	}
	
	SymbolTableItemType tempType = (symbolCode == INTTK) ? INT : CHAR;
	int row = 1;
	int column = 1;
	int dimension = 0;

	getNext();
	if (!hasNext)
	{
		errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
		return false;
	}
	if (symbolCode != IDENFR)
	{
		errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
		return false;
	}

	currentIdentifier = lexicalAnalysis.getToken();

	getNext();

	if (symbolCode != COMMA)
	{
		if (!hasNext || symbolCode != LBRACK)
		{
			if (!symbolTable.push(SymbolTableItem(currentIdentifier, currentDomain, VAR, tempType)))
			{
				errorHandler.error(lexicalAnalysis.getLineCount(), B);
			}
			output.syntaxAnalysisOutput("变量定义无初始化");
			return true;
		}

		dimension++;
		getNext();
		if (!hasNext)
		{
			errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
			return false;
		}
		if (!uinteger())
		{
			errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
			return false;
		}

		column = currentValue;

		if (!hasNext)
		{
			errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
			return false;
		}
		if (symbolCode != RBRACK)
		{
			errorHandler.error(errorLine, M);
			lexicalAnalysis.setAutoComplete();
		}

		getNext();
		if (symbolCode != COMMA)
		{
			if (!hasNext || symbolCode != LBRACK)
			{
				if (!symbolTable.push(SymbolTableItem(currentIdentifier, currentDomain, tempType, dimension, row, column)))
				{
					errorHandler.error(lexicalAnalysis.getLineCount(), B);
				}
				output.syntaxAnalysisOutput("变量定义无初始化");
				return true;
			}

			dimension++;
			getNext();
			if (!hasNext)
			{
				errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
				return false;
			}
			if (!uinteger())
			{
				errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
				return false;
			}

			row = column;
			column = currentValue;

			if (!hasNext)
			{
				errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
				return false;
			}
			if (symbolCode != RBRACK)
			{
				errorHandler.error(errorLine, M);
				lexicalAnalysis.setAutoComplete();
			}

			if (!symbolTable.push(SymbolTableItem(currentIdentifier, currentDomain, tempType, dimension, row, column)))
			{
				errorHandler.error(lexicalAnalysis.getLineCount(), B);
			}
			
			getNext();
		}
	}
	
	if (dimension == 0)
	{
		if (!symbolTable.push(SymbolTableItem(currentIdentifier, currentDomain, tempType, dimension, row, column)))
		{
			errorHandler.error(lexicalAnalysis.getLineCount(), B);
		}
	}

	while (true)
	{
		if (!hasNext || symbolCode != COMMA)
		{
			break;
		}

		dimension = 0;

		getNext();
		if (!hasNext)
		{
			errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
			return false;
		}
		if (symbolCode != IDENFR)
		{
			errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
			return false;
		}

		currentIdentifier = lexicalAnalysis.getToken();

		getNext();

		//特殊判断 ","
		if (hasNext && symbolCode == COMMA)
		{
			if (!symbolTable.push(SymbolTableItem(currentIdentifier, currentDomain, VAR, tempType)))
			{
				errorHandler.error(lexicalAnalysis.getLineCount(), B);
			}
			continue;
		}

		if (!hasNext || symbolCode != LBRACK)
		{
			if (!symbolTable.push(SymbolTableItem(currentIdentifier, currentDomain, VAR, tempType)))
			{
				errorHandler.error(lexicalAnalysis.getLineCount(), B);
			}
			break;
		}

		dimension++;

		getNext();
		if (!hasNext)
		{
			errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
			return false;
		}
		if (!uinteger())
		{
			errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
			return false;
		}

		column = currentValue;

		if (!hasNext)
		{
			errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
			return false;
		}
		if (symbolCode != RBRACK)
		{
			errorHandler.error(errorLine, M);
			lexicalAnalysis.setAutoComplete();
		}

		getNext();
		if (hasNext && symbolCode == COMMA)
		{
			if (!symbolTable.push(SymbolTableItem(currentIdentifier, currentDomain, tempType, dimension, row, column)))
			{
				errorHandler.error(lexicalAnalysis.getLineCount(), B);
			}
			continue;
		}

		if (!hasNext || symbolCode != LBRACK)
		{
			if (!symbolTable.push(SymbolTableItem(currentIdentifier, currentDomain, tempType, dimension, row, column)))
			{
				errorHandler.error(lexicalAnalysis.getLineCount(), B);
			}
			break;
		}

		dimension++;
		getNext();
		if (!hasNext)
		{
			errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
			return false;
		}
		if (!uinteger())
		{
			errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
			return false;
		}

		row = column;
		column = currentValue;

		if (!hasNext)
		{
			errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
			return false;
		}
		if (symbolCode != RBRACK)
		{
			errorHandler.error(errorLine, M);
			lexicalAnalysis.setAutoComplete();
		}

		if (!symbolTable.push(SymbolTableItem(currentIdentifier, currentDomain, tempType, dimension, row, column)))
		{
			errorHandler.error(lexicalAnalysis.getLineCount(), B);
		}
		getNext();
	}
	output.syntaxAnalysisOutput("变量定义无初始化");
	return true;
}

/*＜变量定义及初始化＞ ::= ＜类型标识符＞＜标识符＞=＜常量＞|
＜类型标识符＞＜标识符＞'['＜无符号整数＞']'='{'＜常量＞{,＜常量＞}'}'|
＜类型标识符＞＜标识符＞'['＜无符号整数＞']''['＜无符号整数＞']'=
'{''{'＜常量＞{,＜常量＞}'}'{, '{'＜常量＞{,＜常量＞}'}'}'}'*/
bool SyntaxAnalysis::varWithInit()
{
	int dimension = 0;
	int row = 1, column = 1;

	if (!hasNext || (symbolCode != INTTK && symbolCode != CHARTK))
	{
		return false;
	}

	SymbolTableItemType tempType = (symbolCode == INTTK) ? INT : CHAR;

	getNext();
	if (!hasNext)
	{
		errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
		return false;
	}
	if (symbolCode != IDENFR)
	{
		errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
		return false;
	}
	
	currentIdentifier = lexicalAnalysis.getToken();

	getNext();
	if (!hasNext)
	{
		errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
		return false;
	}

	//一维数组情况
	if (symbolCode == LBRACK)
	{
		dimension++;
		getNext();
		if (!uinteger())
		{
			errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
			return false;
		}
		column = currentValue;

		if (!hasNext)
		{
			errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
			return false;
		}
		if (symbolCode != RBRACK)
		{
			errorHandler.error(errorLine, M);
			lexicalAnalysis.setAutoComplete();
		}

		getNext();
		if (!hasNext)
		{
			errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
			return false;
		}

		//二维数组情况
		if (symbolCode == LBRACK)
		{
			dimension++;
			getNext();
			if (!uinteger())
			{
				errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
				return false;
			}

			row = column;
			column = currentValue;

			if (!hasNext)
			{
				errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
				return false;
			}
			if (symbolCode != RBRACK)
			{
				errorHandler.error(errorLine, M);
				lexicalAnalysis.setAutoComplete();
			}

			SymbolTableItem item = SymbolTableItem(currentIdentifier, currentDomain, tempType, dimension, row, column);

			getNext();
			if (!hasNext)
			{
				errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
				return false;
			}
			if (symbolCode != ASSIGN)
			{
				errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
				return false;
			}

			getNext();
			if (!hasNext)
			{
				errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
				return false;
			}
			if (symbolCode != LBRACE)
			{
				errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
				return false;
			}

			getNext();
			if (!hasNext)
			{
				errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
				return false;
			}
			if (symbolCode != LBRACE)
			{
				errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
				return false;
			}

			getNext();

			SymbolTableItemType constType = constant();
			if (constType == VOID)
			{
				errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
				return false;
			}

			if (constType != tempType)
			{
				errorHandler.error(lexicalAnalysis.getLineCount(), O);
			}
			bool addRes = (tempType == INT)? item.addValue(currentValue) : item.addValue(currentChar);
			
			if (!addRes)
			{
				errorHandler.error(lexicalAnalysis.getLineCount(), N);
			}
			//二维数组: 第一维常量
			while (true)
			{
				if (!hasNext || symbolCode != COMMA)
				{
					break;
				}
				
				getNext();
				SymbolTableItemType constType = constant();
				if (constType == VOID)
				{
					errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
					return false;
				}

				// TODO: 
				bool addRes = (tempType == INT) ? item.addValue(currentValue) : item.addValue(currentChar);

				if (!addRes)
				{
					errorHandler.error(lexicalAnalysis.getLineCount(), N);
				}
			}

			if (!hasNext)
			{
				errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
				return false;
			}
			if (symbolCode != RBRACE)
			{
				errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
				return false;
			}

			//二维数组, 后面的常量赋值
			

			getNext();

			while (true)
			{
				if (!hasNext || symbolCode != COMMA) 
				{
					break;
				}

				item.nextRow();
				getNext();
				if (!hasNext)
				{
					errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
					return false;
				}
				if (symbolCode != LBRACE)
				{
					errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
					return false;
				}

				getNext();
				SymbolTableItemType constType = constant();
				if (constType == VOID)
				{
					errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
					return false;
				}

				if (constType != tempType)
				{
					errorHandler.error(lexicalAnalysis.getLineCount(), O);
				}
				bool addRes = (tempType == INT) ? item.addValue(currentValue) : item.addValue(currentChar);

				if (!addRes)
				{
					errorHandler.error(lexicalAnalysis.getLineCount(), N);
				}

				//二维数组: 常量赋值
				while (true)
				{
					if (!hasNext || symbolCode != COMMA)
					{
						break;
					}

					getNext();
					SymbolTableItemType constType = constant();
					if (constType == VOID)
					{
						errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
						return false;
					}

					if (constType != tempType)
					{
						errorHandler.error(lexicalAnalysis.getLineCount(), O);
					}
					bool addRes = (tempType == INT) ? item.addValue(currentValue) : item.addValue(currentChar);

					if (!addRes)
					{
						errorHandler.error(lexicalAnalysis.getLineCount(), N);
					}
				}

				if (!hasNext)
				{
					errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
					return false;
				}
				if (symbolCode != RBRACE)
				{
					errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
					return false;
				}

				getNext();
			}

			if (!hasNext)
			{
				errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
				return false;
			}
			if (symbolCode != RBRACE)
			{
				errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
				return false;
			}

			if (!item.isFullyAssign())
			{
				errorHandler.error(lexicalAnalysis.getLineCount(), N);
			}
			if (!symbolTable.push(item))
			{
				errorHandler.error(lexicalAnalysis.getLineCount(), B);
			}
			
			getNext();
			output.syntaxAnalysisOutput("变量定义及初始化");
			return true;
		}

		SymbolTableItem item = SymbolTableItem(currentIdentifier, currentDomain, tempType, dimension, row, column);

		if (symbolCode != ASSIGN)
		{
			errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
			return false;
		}

		getNext();
		if (!hasNext)
		{
			errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
			return false;
		}
		if (symbolCode != LBRACE)
		{
			errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
			return false;
		}

		getNext();
		SymbolTableItemType constType = constant();
		if (constType == VOID)
		{
			errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
			return false;
		}

		if (constType != tempType)
		{
			errorHandler.error(lexicalAnalysis.getLineCount(), O);
		}
		bool addRes = (tempType == INT) ? item.addValue(currentValue) : item.addValue(currentChar);

		if (!addRes)
		{
			errorHandler.error(lexicalAnalysis.getLineCount(), N);
		}

		//一维数组: 第一维常量
		while (true)
		{
			if (!hasNext || symbolCode != COMMA)
			{
				break;
			}

			getNext();
			SymbolTableItemType constType = constant();
			if (constType == VOID)
			{
				errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
				return false;
			}

			if (constType != tempType)
			{
				errorHandler.error(lexicalAnalysis.getLineCount(), O);
			}
			bool addRes = (tempType == INT) ? item.addValue(currentValue) : item.addValue(currentChar);

			if (!addRes)
			{
				errorHandler.error(lexicalAnalysis.getLineCount(), N);
			}
		}

		if (!hasNext)
		{
			errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
			return false;
		}
		if (symbolCode != RBRACE) 
		{
			errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
			return false;
		}

		if (!item.isFullyAssign())
		{
			errorHandler.error(lexicalAnalysis.getLineCount(), N);
		}
		if (!symbolTable.push(item))
		{
			errorHandler.error(lexicalAnalysis.getLineCount(), B);
		}
		getNext();
		output.syntaxAnalysisOutput("变量定义及初始化");
		return true;
	}

	if (!hasNext)
	{
		errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
		return false;
	}
	if (symbolCode != ASSIGN)
	{
		errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
		return false;
	}

	getNext();

	SymbolTableItemType constType = constant();
	if (constType == VOID)
	{
		errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
		return false;
	}

	if (constType != tempType)
	{
		errorHandler.error(lexicalAnalysis.getLineCount(), O);
	}

	SymbolTableItem item = (tempType == INT) ? SymbolTableItem(currentIdentifier, currentDomain, VAR, currentValue) :
		SymbolTableItem(currentIdentifier, currentDomain, VAR, currentChar);

	if (!symbolTable.push(item))
	{
		errorHandler.error(lexicalAnalysis.getLineCount(), B);
	}
	output.syntaxAnalysisOutput("变量定义及初始化");
	return true;
}

/*＜有返回值函数定义＞  ::=  ＜声明头部＞'('＜参数表＞')' '{'＜复合语句＞'}' */
bool SyntaxAnalysis::functionWithRet()
{
	if (!hasNext)
	{
		return false;
	}

	SymbolTableItemType temp;
	if (symbolCode == INTTK)
	{
		temp = INT;
	}

	if (symbolCode == CHARTK)
	{
		temp = CHAR;
	}

	if (!declareHeader())
	{
		return false;
	}
	
	currentDomain = currentIdentifier;

	if (symbolCode != LPARENT)
	{
		errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
		return false;
	}

	getNext();
	if (!parameterTable())
	{
		errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
		return false;
	}

	SymbolTableItem function = SymbolTableItem(currentDomain, temp, parameterNumber);
	if (!symbolTable.push(function))
	{
		errorHandler.error(lexicalAnalysis.getLineCount(), B);
	}

	if (!hasNext)
	{
		errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
		return false;
	}
	if (symbolCode != RPARENT)
	{
		errorHandler.error(errorLine, L);
		lexicalAnalysis.setAutoComplete();
	}

	getNext();
	if (symbolCode != LBRACE)
	{
		errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
		return false;
	}
	getNext();

	needRet = true;
	retStatement = false;

	if (!compoundStatement())
	{
		errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
		return false;
	}

	if (!retStatement) {
		errorHandler.error(lexicalAnalysis.getLineCount(), H);
	}

	if (symbolCode != RBRACE)
	{
		errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
		return false;
	}

	getNext();
	
	
	output.syntaxAnalysisOutput("有返回值函数定义");
	return true;
}

/*＜声明头部＞   ::=  int＜标识符＞ |char＜标识符＞*/
bool SyntaxAnalysis::declareHeader()
{
	if (!hasNext)
	{
		return false;
	}

	if (symbolCode != INTTK && symbolCode != CHARTK)
	{
		return false;
	}

	getNext();
	if (symbolCode != IDENFR)
	{
		errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
		return false;
	}

	currentIdentifier = lexicalAnalysis.getToken();

	getNext();
	output.syntaxAnalysisOutput("声明头部");
	return true;
}

/*＜参数表＞ ::=  ＜类型标识符＞＜标识符＞{,＜类型标识符＞＜标识符＞}| ＜空＞ */
bool SyntaxAnalysis::parameterTable()
{
	if (!hasNext)
	{
		return false;
	}

	parameterNumber = 0;

	//空
	if (symbolCode == RPARENT)
	{
		output.syntaxAnalysisOutput("参数表");
		return true;
	}

	if (symbolCode != INTTK && symbolCode != CHARTK)
	{
		output.syntaxAnalysisOutput("参数表");
		return true;
	}

	SymbolTableItemType tempType = (symbolCode == INTTK) ? INT : CHAR;

	getNext();
	
	if (!hasNext)
	{
		errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
		return false;
	}
	if (symbolCode != IDENFR)
	{
		errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
		return false;
	}

	currentIdentifier = lexicalAnalysis.getToken();

	symbolTable.push(SymbolTableItem(currentIdentifier, currentDomain, PARAMETER, tempType));
	parameterNumber++;

	getNext();
	while (true)
	{
		if (!hasNext || symbolCode != COMMA)
		{
			break;
		}

		getNext();
		if (!hasNext)
		{
			errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
			return false;
		}
		if (symbolCode != INTTK && symbolCode != CHARTK)
		{
			errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
			return false;
		}

		tempType = (symbolCode == INTTK) ? INT : CHAR;
		getNext();
		if (!hasNext)
		{
			errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
			return false;
		}
		if (symbolCode != IDENFR)
		{
			errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
			return false;
		}

		currentIdentifier = lexicalAnalysis.getToken();
		symbolTable.push(SymbolTableItem(currentIdentifier, currentDomain, PARAMETER, tempType));
		parameterNumber++;
		getNext();
	}

	output.syntaxAnalysisOutput("参数表");
	return true;
}

/*＜复合语句＞ ::= ［＜常量说明＞］［＜变量说明＞］＜语句列＞*/
bool SyntaxAnalysis::compoundStatement()
{
	constDeclaration();
	varDeclaration();
	if (!statementQueue())
	{
		return false;
	}
	output.syntaxAnalysisOutput("复合语句");
	return true;
}

/*＜无返回值函数定义＞  ::= void＜标识符＞'('＜参数表＞')''{'＜复合语句＞'}' */
bool SyntaxAnalysis::functionWithoutRet()
{
	if (!hasNext || symbolCode != VOIDTK)
	{
		return false;
	}

	getNext();
	if (!hasNext || symbolCode != IDENFR)
	{
		return false;
	}

	currentIdentifier = lexicalAnalysis.getToken();

	currentDomain = currentIdentifier;

	getNext();
	if (!hasNext || symbolCode != LPARENT)
	{
		return false;
	}

	getNext();
	if (!parameterTable())
	{
		return false;
	}

	SymbolTableItem function = SymbolTableItem(currentDomain, VOID, parameterNumber);
	if (!symbolTable.push(function))
	{
		errorHandler.error(lexicalAnalysis.getLineCount(), B);
	}

	if (!hasNext || symbolCode != RPARENT)
	{
		errorHandler.error(errorLine, L);
		lexicalAnalysis.setAutoComplete();
	}

	getNext();
	if (!hasNext || symbolCode != LBRACE)
	{
		return false;
	}

	needRet = false;
	retStatement = false;
	getNext();
	if (!compoundStatement())
	{
		return false;
	}

	if (!hasNext || symbolCode != RBRACE)
	{
		return false;
	}

	getNext();

	retStatement = false;
	output.syntaxAnalysisOutput("无返回值函数定义");
	return true;
}

/*＜语句列＞   ::= ｛＜语句＞｝*/
bool SyntaxAnalysis::statementQueue()
{
	while (true)
	{
		if (!statement())
		{
			break;
		}
	}

	output.syntaxAnalysisOutput("语句列");
	return true;
}

/*＜语句＞    ::= ＜循环语句＞｜＜条件语句＞| ＜有返回值函数调用语句＞;  |＜无返回值函数调用语句＞;｜
＜赋值语句＞;｜＜读语句＞;｜＜写语句＞;｜＜情况语句＞｜＜空＞;|＜返回语句＞; | '{'＜语句列＞'}' */
bool SyntaxAnalysis::statement()
{
	//先处理有冲突的函数调用与赋值语句
	if (symbolCode == IDENFR)
	{
		backup(1199);
		if (assignStatement())
		{
			if (!hasNext || symbolCode != SEMICN)
			{
				errorHandler.error(errorLine, K);
				lexicalAnalysis.setAutoComplete();
			}
			getNext();
			output.syntaxAnalysisOutput("语句");
			return true;
		}
		retract(1210);
		if (callFunction())
		{
			if (!hasNext || symbolCode != SEMICN)
			{
				errorHandler.error(errorLine, K);
				lexicalAnalysis.setAutoComplete();
			}
			getNext();
			output.syntaxAnalysisOutput("语句");
			return true;
		}
		return false;
	}

	if (loopStatement())
	{
		output.syntaxAnalysisOutput("语句");
		return true;
	}

	if (ifStatement())
	{
		output.syntaxAnalysisOutput("语句");
		return true;
	}

	if (switchStatement())
	{
		output.syntaxAnalysisOutput("语句");
		return true;
	}

	if (returnStatement())
	{
		retStatement = true;
		if (symbolCode != SEMICN)
		{
			errorHandler.error(errorLine, K);
			lexicalAnalysis.setAutoComplete();
		}
		getNext();
		output.syntaxAnalysisOutput("语句");
		return true;
	}

	if (scanfStatement())
	{
		if (symbolCode != SEMICN)
		{
			errorHandler.error(errorLine, K);
			lexicalAnalysis.setAutoComplete();
		}
		getNext();
		output.syntaxAnalysisOutput("语句");
		return true;
	}

	if (printfStatement())
	{
		if (symbolCode != SEMICN)
		{
			errorHandler.error(errorLine, K);
			lexicalAnalysis.setAutoComplete();
		}
		getNext();
		output.syntaxAnalysisOutput("语句");
		return true;
	}

	if (symbolCode == LBRACE)
	{
		getNext();
		if (!statementQueue())
		{
			return false;
		}
		if (!hasNext || symbolCode != RBRACE)
		{
			return false;
		}
		getNext();
		output.syntaxAnalysisOutput("语句");
		return true;
	}

	//处理空语句
	if (symbolCode == SEMICN)
	{
		getNext();
		output.syntaxAnalysisOutput("语句");
		return true;
	}

	return false;
}

/*＜循环语句＞ ::=  while '('＜条件＞')'＜语句＞|
				   for'('＜标识符＞＝＜表达式＞;＜条件＞;＜标识符＞＝＜标识符＞(+|-)＜步长＞')'＜语句＞*/
bool SyntaxAnalysis::loopStatement()
{
	if (!hasNext || (symbolCode != WHILETK && symbolCode != FORTK))
	{
		return false;
	}

	if (symbolCode == WHILETK)
	{
		getNext();
		if (!hasNext || symbolCode != LPARENT)
		{
			return false;
		}

		getNext();
		if (!condition())
		{
			return false;
		}

		if (!hasNext || symbolCode != RPARENT)
		{
			errorHandler.error(errorLine, L);
			lexicalAnalysis.setAutoComplete();
		}

		getNext();

		if (!statement())
		{
			return false;
		}
	}

	else if (symbolCode == FORTK)
	{
		getNext();
		if (!hasNext || symbolCode != LPARENT)
		{
			return false;
		}

		getNext();
		if (!hasNext || symbolCode != IDENFR)
		{
			return false;
		}

		currentIdentifier = lexicalAnalysis.getToken();

		if (!symbolTable.hasItem(currentIdentifier, currentDomain))
		{
			errorHandler.error(lexicalAnalysis.getLineCount(), C);
		}

		getNext();
		if (!hasNext || symbolCode != ASSIGN)
		{
			return false;
		}

		getNext();
		if (!expression())
		{
			return false;
		}

		if (!hasNext || symbolCode != SEMICN)
		{
			errorHandler.error(errorLine, K);
			lexicalAnalysis.setAutoComplete();
		}

		getNext();
		if (!condition())
		{
			return false;
		}

		if (!hasNext || symbolCode != SEMICN)
		{
			return false;
		}

		getNext();
		if (!hasNext || symbolCode != IDENFR)
		{
			return false;
		}

		currentIdentifier = lexicalAnalysis.getToken();

		if (!symbolTable.hasItem(currentIdentifier, currentDomain))
		{
			errorHandler.error(lexicalAnalysis.getLineCount(), C);
		}

		getNext();
		if (!hasNext || symbolCode != ASSIGN)
		{
			return false;
		}

		getNext();
		if (!hasNext || symbolCode != IDENFR)
		{
			return false;
		}

		currentIdentifier = lexicalAnalysis.getToken();

		if (!symbolTable.hasItem(currentIdentifier, currentDomain))
		{
			errorHandler.error(lexicalAnalysis.getLineCount(), C);
		}

		getNext();
		if (!hasNext || (symbolCode != PLUS && symbolCode != MINU))
		{
			return false;
		}

		getNext();

		if (!step())
		{
			return false;
		}

		if (!hasNext || symbolCode != RPARENT)
		{
			errorHandler.error(errorLine, L);
			lexicalAnalysis.setAutoComplete();
		}

		getNext();
		if (!statement())
		{
			return false;
		}
	}

	output.syntaxAnalysisOutput("循环语句");
	return true;
}

/*＜步长＞::= ＜无符号整数＞*/
bool SyntaxAnalysis::step()
{
	if (!uinteger())
	{
		return false;
	}

	output.syntaxAnalysisOutput("步长");
	return true;
}

/*＜条件语句＞ ::= if '('＜条件＞')'＜语句＞［else＜语句＞］*/
bool SyntaxAnalysis::ifStatement()
{
	if (!hasNext || symbolCode != IFTK)
	{
		return false;
	}

	getNext();
	if (!hasNext || symbolCode != LPARENT)
	{
		errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
		return false;
	}

	getNext();
	if (!condition())
	{
		errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
		return false;
	}

	if (!hasNext || symbolCode != RPARENT)
	{
		errorHandler.error(errorLine, L);
		lexicalAnalysis.setAutoComplete();
	}

	getNext();
	if (!statement())
	{

		return false;
	}

	if (symbolCode == ELSETK)
	{
		getNext();

		if (!statement())
		{
			return false;
		}
	}

	output.syntaxAnalysisOutput("条件语句");
	return true;
}

/*＜条件＞    ::=  ＜表达式＞＜关系运算符＞＜表达式＞*/
bool SyntaxAnalysis::condition()
{
	if (!expression())
	{
		return false;
	}
	if (exprType != INT)
	{
		errorHandler.error(lexicalAnalysis.getLineCount(), F);
	}
	if (!relationOp())
	{
		return false;
	}
	if (!expression())
	{
		return false;
	}
	if (exprType != INT)
	{
		errorHandler.error(lexicalAnalysis.getLineCount(), F);
	}

	output.syntaxAnalysisOutput("条件");
	return true;
}

/*＜表达式＞ ::= ［＋｜－］＜项＞{＜加法运算符＞＜项＞}*/
bool SyntaxAnalysis::expression()
{
	if (!hasNext)
	{
		return false;
	}

	exprType = INT;
	string functionName = callFunctionName;
	int paraN = parameterNumber;

	bool flag = false;

	if (symbolCode == PLUS || symbolCode == MINU)
	{
		getNext();
	}

	if (!term())
	{
		return false;
	}
	while (true)
	{
		if (symbolCode != PLUS && symbolCode != MINU)
		{
			if (flag)
			{
				exprType = INT;
			}
			break;
		}

		flag = true;
		exprType = INT;
		getNext();
		if (!term())
		{
			return false;
		}
	}
	callFunctionName = functionName;
	parameterNumber = paraN;
	output.syntaxAnalysisOutput("表达式");
	return true;
}

/*＜项＞ ::= ＜因子＞{＜乘法运算符＞＜因子＞} */
bool SyntaxAnalysis::term()
{
	bool flag = false;
	if (!factor())
	{
		return false;
	}
	while (true)
	{
		if (symbolCode != MULT && symbolCode != DIV)
		{
			if (flag)
			{
				exprType = INT;
			}
			break;
		}

		flag = true;
		exprType = INT;
		getNext();
		if (!factor())
		{

			return false;
		}
	}
	output.syntaxAnalysisOutput("项");
	return true;
}

/*＜因子＞ ::= ＜标识符＞｜＜标识符＞'['＜表达式＞']'|＜标识符＞'['＜表达式＞']''['＜表达式＞']'|
'('＜表达式＞')'｜＜整数＞|＜字符＞｜＜有返回值函数调用语句＞*/
bool SyntaxAnalysis::factor()
{	
	if (!hasNext)
	{
		return false;
	}

	backup(1572);	

	//特殊判断, 是不是函数?
	if (symbolCode == IDENFR)
	{
		currentIdentifier = lexicalAnalysis.getToken();
		getNext();
		if (!hasNext)
		{
			if (!symbolTable.hasItem(currentIdentifier, currentDomain))
			{
				errorHandler.error(lexicalAnalysis.getLineCount(), C);
			}
			else
			{
				exprType = symbolTable.getCurrentType();
			}
			output.syntaxAnalysisOutput("因子", 1574);
			return true;
		}

		if (symbolCode != LPARENT)
		{
			if (!symbolTable.hasItem(currentIdentifier, currentDomain))
			{
				errorHandler.error(lexicalAnalysis.getLineCount(), C);
			}
			else
			{
				exprType = symbolTable.getCurrentType();
			}

			SymbolTableItemType temp = exprType;
			if (symbolCode == LBRACK)
			{
				getNext();
				if (!expression())
				{
					return false;
				}

				if (exprType != INT)
				{
					errorHandler.error(lexicalAnalysis.getLineCount(), I);
				}
				if (!hasNext)
				{
					return false;
				}
				if (symbolCode != RBRACK)
				{
					errorHandler.error(errorLine, M);
					lexicalAnalysis.setAutoComplete();
				}

				getNext();
				if (!hasNext)
				{
					return true;
				}

				if (symbolCode == LBRACK)
				{
					getNext();
					if (!expression())
					{
						return false;
					}

					if (exprType != INT)
					{
						errorHandler.error(lexicalAnalysis.getLineCount(), I);
					}

					if (!hasNext)
					{
						return false;
					}

					if (symbolCode != RBRACK)
					{
						errorHandler.error(errorLine, M);
						lexicalAnalysis.setAutoComplete();
					}
					getNext();
					exprType = temp;
					output.syntaxAnalysisOutput("因子", 1621);
					return true;
				}
				exprType = temp;
				output.syntaxAnalysisOutput("因子", 1624);
				return true;
			}
			output.syntaxAnalysisOutput("因子", 1628);
			return true;
		}
		else
		{
			retract(1638);
			SymbolTableItemType temp = symbolTable.getFunctionType(currentIdentifier);
			if (callFunction())
			{
				if (temp != VOID)
				{
					exprType = temp;
				}
				output.syntaxAnalysisOutput("因子", 1636);
				return true;
			}
			return false;
		}
		
	}
	if (symbolCode == LPARENT)
	{
		getNext();
		if (!expression())
		{
			return false;
		}
		
		exprType = INT;
		if (!hasNext)
		{
			return false;
		}
		if (symbolCode != RPARENT)
		{
			errorHandler.error(errorLine, L);
			lexicalAnalysis.setAutoComplete();
		}
		
		getNext();
		output.syntaxAnalysisOutput("因子", 1660);
		return true;
	}
	if (integer() || character())
	{
		output.syntaxAnalysisOutput("因子", 1665);
		return true;
	}

	return false;
}

// TODO: 填符号表区分

/*＜有返回值函数调用语句＞ ::= ＜标识符＞'('＜值参数表＞')' */
/*＜无返回值函数调用语句＞ ::= ＜标识符＞'('＜值参数表＞')' */
bool SyntaxAnalysis::callFunction()
{
	if (!hasNext || symbolCode != IDENFR)
	{
		return false;
	}

	currentIdentifier = lexicalAnalysis.getToken();
	callFunctionName = currentIdentifier;

	bool hasFunction = symbolTable.hasFunction(currentIdentifier);
	if (!hasFunction)
	{
		errorHandler.error(lexicalAnalysis.getLineCount(), C);
	}

	SymbolTableItem& function = symbolTable.getCurrent();

	getNext();
	if (symbolCode != LPARENT)
	{
		return false;
	}

	getNext();
	if (!parameterValueTable())
	{
		return false;
	}

	if (!hasNext || symbolCode != RPARENT)
	{
		errorHandler.error(errorLine, L);
		lexicalAnalysis.setAutoComplete();
	}

	if (hasFunction && parameterNumber != function.getParaCount())
	{
		errorHandler.error(lexicalAnalysis.getLineCount(), D);
	}

	getNext();
	if (symbolTable.hasRet(currentDomain))
	{
		output.syntaxAnalysisOutput("有返回值函数调用语句");
	}
	else
	{
		output.syntaxAnalysisOutput("无返回值函数调用语句");
	}
	return true;
}

/*＜值参数表＞ ::= ＜表达式＞{,＜表达式＞}｜＜空＞*/
bool SyntaxAnalysis::parameterValueTable()
{
	if (!hasNext)
	{
		return false;
	}

	// TODO:
	parameterNumber = 0;
	//空, 特殊处理 不预读
	if (symbolCode == RPARENT)
	{
		output.syntaxAnalysisOutput("值参数表");
		return true;
	}

	bool hasPara = symbolTable.findParameter(callFunctionName);
	SymbolTableItemType paraType = symbolTable.getCurrentType();

	if (!expression())
	{
		output.syntaxAnalysisOutput("值参数表");
		return true;
	}

	if (hasPara && paraType != exprType)
	{
		errorHandler.error(lexicalAnalysis.getLineCount(), E);
	}
	parameterNumber++;

	while (true)
	{
		if (!hasNext || symbolCode != COMMA)
		{
			break;
		}

		hasPara = symbolTable.nextParameter(callFunctionName);
		paraType = symbolTable.getCurrentType();
		getNext();
		if (!expression())
		{
			return false;
		}
		if (hasPara && paraType != exprType)
		{
			errorHandler.error(lexicalAnalysis.getLineCount(), E);
		}
		parameterNumber++;
	}

	output.syntaxAnalysisOutput("值参数表");
	return true;
}

/*＜赋值语句＞ ::= ＜标识符＞＝＜表达式＞|＜标识符＞'['＜表达式＞']'=＜表达式＞|
				  ＜标识符＞'['＜表达式＞']''['＜表达式＞']' =＜表达式＞*/
bool SyntaxAnalysis::assignStatement()
{
	if (!hasNext || symbolCode != IDENFR)
	{
		return false;
	}

	SymbolTableItemKind kind = VAR;

	currentIdentifier = lexicalAnalysis.getToken();

	if (!symbolTable.hasItem(currentIdentifier, currentDomain))
	{
		errorHandler.error(lexicalAnalysis.getLineCount(), C);
	}
	else
	{
		kind = symbolTable.getCurrentKind();
	}

	getNext();
	if (hasNext && symbolCode == LBRACK)
	{
		getNext();
		if (!expression())
		{
			return false;
		}

		if (exprType != INT)
		{
			errorHandler.error(lexicalAnalysis.getLineCount(), I);
		}
		if (!hasNext || symbolCode != RBRACK)
		{
			errorHandler.error(errorLine, M);
			lexicalAnalysis.setAutoComplete();
		}

		getNext();
		if (hasNext && symbolCode == LBRACK)
		{
			getNext();
			if (!expression())
			{
				return false;
			}

			if (exprType != INT)
			{
				errorHandler.error(lexicalAnalysis.getLineCount(), I);
			}

			if (!hasNext || symbolCode != RBRACK)
			{
				errorHandler.error(errorLine, M);
				lexicalAnalysis.setAutoComplete();
			}

			getNext();
			if (!hasNext || symbolCode != ASSIGN)
			{
				return false;
			}

			getNext();
			if (!expression())
			{
				return false;
			}

			if (kind == CONST)
			{
				errorHandler.error(lexicalAnalysis.getLineCount(), J);
			}
			output.syntaxAnalysisOutput("赋值语句");
			return true;
		}
		
		if (!hasNext || symbolCode != ASSIGN)
		{
			return false;
		}

		getNext();
		if (!expression())
		{
			return false;
		}

		if (kind == CONST)
		{
			errorHandler.error(lexicalAnalysis.getLineCount(), J);
		}
		output.syntaxAnalysisOutput("赋值语句");
		return true;
	}

	if (!hasNext || symbolCode != ASSIGN)
	{
		return false;
	}

	getNext();
	if (!expression())
	{
		return false;
	}

	if (kind == CONST)
	{
		errorHandler.error(lexicalAnalysis.getLineCount(), J);
	}
	output.syntaxAnalysisOutput("赋值语句");
	return true;
}

/*＜读语句＞ ::=  scanf '('＜标识符＞')' */
bool SyntaxAnalysis::scanfStatement()
{
	if (!hasNext || symbolCode != SCANFTK)
	{
		return false;
	}

	SymbolTableItemKind kind = VAR;

	getNext();
	if (!hasNext || symbolCode != LPARENT)
	{
		return false;
	}

	getNext();
	if (!hasNext || symbolCode != IDENFR)
	{
		return false;
	}

	currentIdentifier = lexicalAnalysis.getToken();

	if (!symbolTable.hasItem(currentIdentifier, currentDomain))
	{
		errorHandler.error(lexicalAnalysis.getLineCount(), C);
	}
	else
	{
		kind = symbolTable.getCurrentKind();
	}

	getNext();
	if (!hasNext || symbolCode != RPARENT)
	{
		errorHandler.error(errorLine, L);
		lexicalAnalysis.setAutoComplete();
	}

	getNext();

	if (kind == CONST)
	{
		errorHandler.error(lexicalAnalysis.getLineCount(), J);
	}
	output.syntaxAnalysisOutput("读语句");
	return true;
}

/*＜写语句＞ ::= printf '(' ＜字符串＞,＜表达式＞ ')'| printf '('＜字符串＞ ')'| printf '('＜表达式＞')' */
bool SyntaxAnalysis::printfStatement()
{
	if (!hasNext || symbolCode != PRINTFTK)
	{
		return false;
	}

	getNext();
	if (!hasNext || symbolCode != LPARENT)
	{
		return false;
	}

	getNext();

	if (myString())
	{
		if (symbolCode == COMMA)
		{
			getNext();
			if (!expression())
			{
				return false;
			}
			if (symbolCode != RPARENT)
			{
				return false;
			}
			getNext();
			output.syntaxAnalysisOutput("写语句", 1909);
			return true;
		}
		if (symbolCode != RPARENT)
		{
			return false;
		}
		getNext();
		output.syntaxAnalysisOutput("写语句", 1917);
		return true;
	}
	if (!expression())
	{
		return false;
	}

	if (symbolCode != RPARENT)
	{
		errorHandler.error(errorLine, L);
		lexicalAnalysis.setAutoComplete();
	}

	getNext();
	output.syntaxAnalysisOutput("写语句", 1924);
	return true;
}

/*＜情况语句＞  ::=  switch ‘(’＜表达式＞‘)’ ‘{’＜情况表＞＜缺省＞‘}’*/
bool SyntaxAnalysis::switchStatement()
{
	if (!hasNext || symbolCode != SWICHTK)
	{
		return false;
	}

	getNext();
	if (!hasNext || symbolCode != LPARENT)
	{
		return false;
	}

	getNext();
	if (!expression())
	{
		return false;
	}

	if (!hasNext || symbolCode != RPARENT)
	{
		errorHandler.error(errorLine, L);
		lexicalAnalysis.setAutoComplete();
	}

	getNext();
	if (!hasNext || symbolCode != LBRACE)
	{
		return false;
	}

	getNext();
	if (!caseTable())
	{
		return false;
	}
	if (!defaultStatement())
	{
		errorHandler.error(lexicalAnalysis.getLineCount(), P);
	}

	if (!hasNext || symbolCode != RBRACE)
	{
		return false;
	}

	getNext();
	output.syntaxAnalysisOutput("情况语句");
	return true;
}

/*＜情况表＞   ::=  ＜情况子语句＞{＜情况子语句＞} */
bool SyntaxAnalysis::caseTable()
{
	SymbolTableItemType temp = exprType;
	if (!caseStatement(temp))
	{
		return false;
	}
	while (true)
	{
		if (!caseStatement(temp))
		{
			break;
		}
	}
	
	output.syntaxAnalysisOutput("情况表");
	return true;
}

/*＜情况子语句＞  ::=  case＜常量＞：＜语句＞ */
bool SyntaxAnalysis::caseStatement(SymbolTableItemType type)
{
	if (!hasNext || symbolCode != CASETK)
	{
		return false;
	}

	getNext();

	SymbolTableItemType constType = constant();

	if (constType == VOID)
	{
		return false;
	}
	else
	{
		if (type != constType)
		{
			errorHandler.error(lexicalAnalysis.getLineCount(), O);
		}
	}

	if (!hasNext || symbolCode != COLON)
	{
		return false;
	}

	getNext();
	if (!statement())
	{
		return false;
	}

	output.syntaxAnalysisOutput("情况子语句");
	return true;
}

/*＜缺省＞   ::=  default :＜语句＞ */
bool SyntaxAnalysis::defaultStatement()
{
	if (!hasNext || symbolCode != DEFAULTTK)
	{
		return false;
	}

	getNext();
	if (!hasNext || symbolCode != COLON)
	{
		return false;
	}

	getNext();
	if (!statement())
	{
		return false;
	}

	output.syntaxAnalysisOutput("缺省");
	return true;
}

/*＜返回语句＞   ::=  return['('＜表达式＞')']*/
bool SyntaxAnalysis::returnStatement()
{
	if (!hasNext || symbolCode != RETURNTK)
	{
		return false;
	}

	SymbolTableItemType temp = VOID;
	getNext();
	if (symbolCode == LPARENT)
	{
		getNext();
		if (!expression())
		{
			return false;
		}
		
		if (needRet)
		{
			temp = exprType;
			SymbolTableItemType functionType = symbolTable.getFunctionType(currentDomain);
			if (temp != functionType)
			{
				errorHandler.error(lexicalAnalysis.getLineCount(), H);
			}
		}
		else
		{
			errorHandler.error(lexicalAnalysis.getLineCount(), G);
		}

		if (!hasNext || symbolCode != RPARENT)
		{
			errorHandler.error(errorLine, L);
			lexicalAnalysis.setAutoComplete();
		}

		getNext();
		retStatement = true;
		output.syntaxAnalysisOutput("返回语句");
		return true;
	}

	if (needRet)
	{
		errorHandler.error(lexicalAnalysis.getLineCount(), H);
	}
	retStatement = true;
	output.syntaxAnalysisOutput("返回语句");
	return true;
}

/*＜整数＞ ::= ［＋｜－］＜无符号整数＞ */
bool SyntaxAnalysis::integer()
{
	bool isMinus = false;

	if (!hasNext || (symbolCode != PLUS && symbolCode != MINU && symbolCode != INTCON))
	{
		return false;
	}

	if (symbolCode == PLUS || symbolCode ==  MINU) 
	{
		getNext();
		if (!uinteger())
		{
			errorHandler.syntaxError(lexicalAnalysis.getLineCount(), __func__);
			return false;
		}
		isMinus = (symbolCode == (PLUS)) ? false : true;
	}

	else if (!uinteger())
	{
		return false;
	}

	currentValue = (isMinus) ? currentValue : currentValue;
	output.syntaxAnalysisOutput("整数");
	return true;
}

bool SyntaxAnalysis::uinteger()
{
	if (!hasNext)
	{
		return false;
	}
	if (symbolCode != INTCON)
	{
		return false;
	}
	currentValue = lexicalAnalysis.getValue();

	getNext();
	output.syntaxAnalysisOutput("无符号整数");
	return true;
}

SymbolTableItemType SyntaxAnalysis::constant()
{
	currentValue = 0;
	currentChar = '\0';
	if (!hasNext)
	{
		return VOID;
	}
	if (symbolCode != CHARCON)
	{
		if (!integer())
		{
			return VOID;
		}
		else
		{
			output.syntaxAnalysisOutput("常量");
			return INT;
		}
	}
	else
	{
		currentChar = lexicalAnalysis.getToken().c_str()[0];
		
		getNext();
		output.syntaxAnalysisOutput("常量");
		return CHAR;
	}
}

bool SyntaxAnalysis::character()
{
	if (!hasNext)
	{
		return false;
	}

	if (symbolCode == CHARCON && lexicalAnalysis.isCharacter())
	{
		exprType = CHAR;
		getNext();
		return true;
	}

	return false;
}

/*＜字符串＞ ::=  "｛十进制编码为32,33,35-126的ASCII字符｝"*/
bool SyntaxAnalysis::myString()
{
	if (!hasNext || symbolCode != STRCON)
	{
		return false;
	}

	getNext();
	output.syntaxAnalysisOutput("字符串");
	return true;
}

/*＜关系运算符＞  ::=  <｜<=｜>｜>=｜!=｜== */
bool SyntaxAnalysis::relationOp()
{
	if (symbolCode == LSS || symbolCode == LEQ || symbolCode == GRE ||
		symbolCode == GEQ || symbolCode == NEQ || symbolCode == EQL)
	{
		getNext();
		return true;
	}
	return false;
}