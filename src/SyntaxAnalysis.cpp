#include"SyntaxAnalysis.h"
#include<iostream>

using namespace std;

SyntaxAnalysis::SyntaxAnalysis(ErrorHandler& errorHandler, LexicalAnalysis& lexicalAnalysis, Output& output)
	:errorHandler(errorHandler), lexicalAnalysis(lexicalAnalysis), output(output)
{

}

void SyntaxAnalysis::getNext()
{
	hasNext = lexicalAnalysis.getNextSymbol();
	symbolCode = lexicalAnalysis.getSymbolCode();
}

void SyntaxAnalysis::backup()
{
	symbolCodeBackup = symbolCode;
	lexicalAnalysis.backup();
	output.backup();
}

void SyntaxAnalysis::retract()
{
	symbolCode = symbolCodeBackup;
	lexicalAnalysis.restore();
	output.retract();
}

SymbolCode SyntaxAnalysis::getSymbolCode()
{
	return lexicalAnalysis.getSymbolCode();
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
}

/*＜程序＞ ::= ［＜常量说明＞］［＜变量说明＞］{ ＜有返回值函数定义＞ | ＜无返回值函数定义＞ }＜主函数＞*/
bool SyntaxAnalysis::program()
{
	if (!hasNext)
	{
		return false;
	}
	constDeclaration();
	varDeclaration(true);
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
		errorHandler.syntaxError();
		return false;
	}

	if (!constDefinition())
	{
		errorHandler.syntaxError();
		return false;
	}

	// 未分析结束, 文件已结束
	if (!hasNext)
	{
		errorHandler.syntaxError();
		return false;
	}
	// 缺失分号, 自动补全, 继续分析
	if (symbolCode != SEMICN)
	{
		errorHandler.syntaxError();
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
			errorHandler.syntaxError();
			return false;
		}
		if (!hasNext)
		{
			errorHandler.syntaxError();
			return false;
		}
		if (symbolCode != SEMICN)
		{
			errorHandler.syntaxError();
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
			errorHandler.syntaxError();
			return false;
		}

		getNext();
		if (symbolCode != ASSIGN)
		{
			errorHandler.syntaxError();
			return false;
		}

		getNext();

		if (!integer())
		{
			errorHandler.syntaxError();
			return false;
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
				errorHandler.syntaxError();
				return false;
			}

			if (symbolCode != IDENFR)
			{
				errorHandler.syntaxError();
				return false;
			}

			getNext();
			if (!hasNext)
			{
				errorHandler.syntaxError();
				return false;
			}

			if (symbolCode != ASSIGN)
			{
				errorHandler.syntaxError();
				return false;
			}

			getNext();
			if (!integer())
			{
				errorHandler.syntaxError();
				return false;
			}
		}
	}

	else if (symbolCode == CHARTK)
	{
		getNext();
		if (symbolCode != IDENFR)
		{
			errorHandler.syntaxError();
			return false;
		}

		getNext();
		if (symbolCode != ASSIGN)
		{
			errorHandler.syntaxError();
			return false;
		}

		getNext();

		if (!hasNext)
		{
			errorHandler.syntaxError();
			return false;
		}

		if (symbolCode != CHARCON)
		{
			errorHandler.syntaxError();
			return false;
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
				errorHandler.syntaxError();
				return false;
			}

			if (symbolCode != IDENFR)
			{
				errorHandler.syntaxError();
				return false;
			}

			getNext();
			if (!hasNext)
			{
				errorHandler.syntaxError();
				return false;
			}
			if (symbolCode != ASSIGN)
			{
				errorHandler.syntaxError();
				return false;
			}

			getNext();
			if (symbolCode != CHARCON)
			{
				errorHandler.syntaxError();
				return false;
			}

			getNext();
		}
	}
	output.syntaxAnalysisOutput("常量定义");
	return true;
}

/*＜变量说明＞ ::= ＜变量定义＞;{＜变量定义＞;} */
bool SyntaxAnalysis::varDeclaration(bool isGlobal)
{
	if (!hasNext)
	{
		return false;
	}

	backup();

	if (isGlobal)
	{
		if (symbolCode == CHARTK || symbolCode == INTTK)
		{
			getNext();
			if (hasNext && symbolCode == IDENFR)
			{
				getNext();
				if (hasNext && symbolCode == LPARENT)
				{
					retract();
					return false;
				}
			}
		}
		retract();
	}

	if (!varDefinition())
	{
		return false;
	}

	if (symbolCode != SEMICN)
	{
		errorHandler.syntaxError();
		lexicalAnalysis.setAutoComplete();
	}

	getNext();

	while (true)
	{
		backup();

		if (isGlobal)
		{
			if (symbolCode == CHARTK || symbolCode == INTTK)
			{
				getNext();
				if (hasNext && symbolCode == IDENFR)
				{
					getNext();
					if (hasNext && symbolCode == LPARENT)
					{
						retract();
						break;
					}
				}
			}
			retract();
		}

		if (!hasNext || !varDefinition())
		{
			break;
		}

		if (symbolCode != SEMICN)
		{
			errorHandler.syntaxError();
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
	backup();
	if (!varWithInit())
	{
		retract();
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
	
	getNext();
	if (!hasNext)
	{
		errorHandler.syntaxError();
		return false;
	}
	if (symbolCode != IDENFR)
	{
		errorHandler.syntaxError();
		return false;
	}

	getNext();
	if (!hasNext || symbolCode != LBRACK)
	{
		output.syntaxAnalysisOutput("变量定义无初始化");
		return true;
	}

	getNext();
	if (!hasNext)
	{
		errorHandler.syntaxError();
		return false;
	}
	if (symbolCode != INTCON)
	{
		errorHandler.syntaxError();
		return false;
	}

	getNext();
	if (!hasNext)
	{
		errorHandler.syntaxError();
		return false;
	}
	if (symbolCode != RBRACK)
	{
		errorHandler.syntaxError();
		return false;
	}

	getNext();
	if (!hasNext || symbolCode != LBRACK)
	{
		output.syntaxAnalysisOutput("变量定义无初始化");
		return true;
	}

	getNext();
	if (!hasNext)
	{
		errorHandler.syntaxError();
		return false;
	}
	if (symbolCode != INTCON)
	{
		errorHandler.syntaxError();
		return false;
	}
	
	getNext();
	if (!hasNext)
	{
		errorHandler.syntaxError();
		return false;
	}
	if (symbolCode != RBRACK)
	{
		errorHandler.syntaxError();
		return false;
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
			errorHandler.syntaxError();
			return false;
		}
		if (symbolCode != IDENFR)
		{
			errorHandler.syntaxError();
			return false;
		}

		getNext();
		if (!hasNext || symbolCode != LBRACK)
		{
			break;
		}

		getNext();
		if (!hasNext)
		{
			errorHandler.syntaxError();
			return false;
		}
		if (symbolCode != INTCON)
		{
			errorHandler.syntaxError();
			return false;
		}

		getNext();
		if (!hasNext)
		{
			errorHandler.syntaxError();
			return false;
		}
		if (symbolCode != RBRACK)
		{
			errorHandler.syntaxError();
			return false;
		}

		getNext();
		if (!hasNext || symbolCode != LBRACK)
		{
			break;
		}

		getNext();
		if (!hasNext)
		{
			errorHandler.syntaxError();
			return false;
		}
		if (symbolCode != INTCON)
		{
			errorHandler.syntaxError();
			return false;
		}

		getNext();
		if (!hasNext)
		{
			errorHandler.syntaxError();
			return false;
		}
		if (symbolCode != RBRACK)
		{
			errorHandler.syntaxError();
			return false;
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

	if (!hasNext || (symbolCode != INTTK && symbolCode != CHARTK))
	{
		return false;
	}

	getNext();
	if (!hasNext)
	{
		errorHandler.syntaxError();
		return false;
	}
	if (symbolCode != IDENFR)
	{
		errorHandler.syntaxError();
		return false;
	}
	
	getNext();
	if (!hasNext)
	{
		errorHandler.syntaxError();
		return false;
	}

	//一维数组情况
	if (symbolCode == LBRACK)
	{
		dimension++;
		getNext();
		if (!uinteger())
		{
			errorHandler.syntaxError();
			return false;
		}

		if (!hasNext)
		{
			errorHandler.syntaxError();
			return false;
		}
		if (symbolCode != RBRACK)
		{
			errorHandler.syntaxError();
			return false;
		}

		getNext();
		if (!hasNext)
		{
			errorHandler.syntaxError();
			return false;
		}

		//二维数组情况
		if (symbolCode == LBRACK)
		{
			dimension++;
			getNext();
			if (!uinteger())
			{
				errorHandler.syntaxError();
				return false;
			}
			if (!hasNext)
			{
				errorHandler.syntaxError();
				return false;
			}
			if (symbolCode != RBRACK)
			{
				errorHandler.syntaxError();
				return false;
			}

			getNext();
			if (!hasNext)
			{
				errorHandler.syntaxError();
				return false;
			}
			if (symbolCode != ASSIGN)
			{
				errorHandler.syntaxError();
				return false;
			}

			getNext();
			if (!hasNext)
			{
				errorHandler.syntaxError();
				return false;
			}
			if (symbolCode != LBRACE)
			{
				errorHandler.syntaxError();
				return false;
			}

			getNext();
			if (!hasNext)
			{
				errorHandler.syntaxError();
				return false;
			}
			if (symbolCode != LBRACE)
			{
				errorHandler.syntaxError();
				return false;
			}

			getNext();
			if (!constant())
			{
				errorHandler.syntaxError();
				return false;
			}

			//二维数组: 第一维常量
			while (true)
			{
				if (!hasNext || symbolCode != COMMA)
				{
					break;
				}
				
				getNext();
				if (!constant())
				{
					errorHandler.syntaxError();
					return false;
				}
			}

			if (!hasNext)
			{
				errorHandler.syntaxError();
				return false;
			}
			if (symbolCode != RBRACE)
			{
				errorHandler.syntaxError();
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

				getNext();
				if (!hasNext)
				{
					errorHandler.syntaxError();
					return false;
				}
				if (symbolCode != LBRACE)
				{
					errorHandler.syntaxError();
					return false;
				}

				getNext();
				if (!constant())
				{
					errorHandler.syntaxError();
					return false;
				}

				//二维数组: 常量赋值
				while (true)
				{
					if (!hasNext || symbolCode != COMMA)
					{
						break;
					}

					getNext();
					if (!constant())
					{
						errorHandler.syntaxError();
						return false;
					}
				}

				if (!hasNext)
				{
					errorHandler.syntaxError();
					return false;
				}
				if (symbolCode != RBRACE)
				{
					errorHandler.syntaxError();
					return false;
				}

				getNext();
			}

			if (!hasNext)
			{
				errorHandler.syntaxError();
				return false;
			}
			if (symbolCode != RBRACE)
			{
				errorHandler.syntaxError();
				return false;
			}

			getNext();

			output.syntaxAnalysisOutput("变量定义及初始化");
			return true;
		}

		if (symbolCode != ASSIGN)
		{
			errorHandler.syntaxError();
			return false;
		}

		getNext();
		if (!hasNext)
		{
			errorHandler.syntaxError();
			return false;
		}
		if (symbolCode != LBRACE)
		{
			errorHandler.syntaxError();
			return false;
		}

		getNext();
		if (!constant())
		{
			errorHandler.syntaxError();
			return false;
		}

		//一维数组: 第一维常量
		while (true)
		{
			if (!hasNext || symbolCode != COMMA)
			{
				break;
			}

			getNext();
			if (!constant())
			{
				errorHandler.syntaxError();
				return false;
			}
		}

		if (!hasNext)
		{
			errorHandler.syntaxError();
			return false;
		}
		if (symbolCode != RBRACE)
		{
			errorHandler.syntaxError();
			return false;
		}
		getNext();
		output.syntaxAnalysisOutput("变量定义及初始化");
		return true;
	}

	if (!hasNext)
	{
		errorHandler.syntaxError();
		return false;
	}
	if (symbolCode != ASSIGN)
	{
		errorHandler.syntaxError();
		return false;
	}

	getNext();

	if (!constant())
	{
		errorHandler.syntaxError();
		return false;
	}
	
	output.syntaxAnalysisOutput("变量定义及初始化");
	return true;
}

/*＜有返回值函数定义＞  ::=  ＜声明头部＞'('＜参数表＞')' '{'＜复合语句＞'}' */
bool SyntaxAnalysis::functionWithRet()
{
	return true;
}

/*＜无返回值函数定义＞  ::= void＜标识符＞'('＜参数表＞')''{'＜复合语句＞'}' */
bool SyntaxAnalysis::functionWithoutRet()
{
	return true;
}

/*＜整数＞ ::= ［＋｜－］＜无符号整数＞ */
bool SyntaxAnalysis::integer()
{
	bool isMinus;

	if (!hasNext || (symbolCode != PLUS && symbolCode != MINU && symbolCode != INTCON))
	{
		return false;
	}

	if (symbolCode == PLUS || symbolCode ==  MINU) 
	{
		getNext();
		if (!uinteger())
		{
			errorHandler.syntaxError();
			return false;
		}
		isMinus = (symbolCode == (PLUS))? false : true;
	}

	else if (!uinteger())
	{
		return false;
	}

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
	getNext();
	output.syntaxAnalysisOutput("无符号整数");
	return true;
}

bool SyntaxAnalysis::constant()
{
	if (!hasNext)
	{
		return false;
	}
	bool isInteger = false;
	if (symbolCode != CHARCON)
	{
		isInteger = integer();
		if (!isInteger)
		{
			return false;
		}
	}
	if (!isInteger)
	{
		getNext();
	}
	output.syntaxAnalysisOutput("常量");
	return true;
}
