#include"LexicalAnalysis.h"
#include<string>
#include<fstream>
#include<cstdlib>
#include<iostream>
#include<algorithm>
#include"charJudge.h"

LexicalAnalysis::LexicalAnalysis(Output& output):output(output)
{
	fileLength = 0;		
	index = 0;				
	c = '\0';
	finish = true;
	line = 1;
	value = 0;
	token.clear();
	fileContent.clear();
}

void LexicalAnalysis::getChar() 
{
	if (finish)
	{
		c = EOF;
	}
	else
	{
		c = fileContent[index++];
	}
	if (index >= fileLength) 
	{
		finish = true;
	}
}

void LexicalAnalysis::retract()
{
	index--;
}

void LexicalAnalysis::readFile(string filename)
{
	fstream in(filename, ios::in);
	if (!in)
	{
		cout << "Open File Failed!" << endl;
		exit(EXIT_SUCCESS);
	}
	istreambuf_iterator<char>isb(in), end;
	fileContent = string(isb, end);
	in.close();
	fileLength = fileContent.size();
	if (index < fileLength)
	{
		finish = false;
	}
	else
	{
		finish = true;
	}
	cout << "Read File Succeed! File Size is " << fileLength << " Bytes." << endl;
}

SymbolCode LexicalAnalysis::getSymbolCode()
{
	return symbolCode;
}

string LexicalAnalysis::getToken()
{
	return token;
}

int LexicalAnalysis::getLineCount()
{
	return line;
}

SymbolCode LexicalAnalysis::reserver() {
	string str = token;
	transform(str.begin(), str.end(), str.begin(), ::tolower);
	if (str == "if") 
	{
		return IFTK;
	}
	else if (str == "else") 
	{
		return ELSETK;
	}
	else if (str == "for") 
	{
		return FORTK;
	}
	else if (str == "while") 
	{
		return WHILETK;
	}
	else if (str == "int") 
	{
		return INTTK;
	}
	else if (str == "char") 
	{
		return CHARTK;
	}
	else if (str == "printf") 
	{
		return PRINTFTK;
	}
	else if (str == "scanf") 
	{
		return SCANFTK;
	}
	else if (str == "case") 
	{
		return CASETK;
	}
	else if (str == "switch") 
	{
		return SWICHTK;
	}
	else if (str == "default") 
	{
		return DEFAULTTK;
	}
	else if (str == "const") 
	{
		return CONSTTK;
	}
	else if (str == "void") 
	{
		return VOIDTK;
	}
	else if (str == "return") 
	{
		return RETURNTK;
	}
	else if (str == "main") 
	{
		return MAINTK;
	}
	return IDENFR;
}

bool LexicalAnalysis::getNextSymbol() 
{
	if (!token.empty())
	{
		output.lexicalAnalysisOutput(symbolCode, token);
	}

	//自动补全
	if (autoComplete)
	{
		autoComplete = false;
		return true;
	}

	//清空token，读取第一个字符
	token.clear();
	getChar();

	//跳过空白符
	while (isspace(c)) 
	{
		if (isNewline(c)) 
		{
			line++;
		}
		getChar();
		//文件末尾全是空白符，返回
	}

	//判断是不是字母
	if (isLetter(c)) 
	{
		//将字符拼接成字符串
		while (isLetter(c) || isDigit(c)) 
		{
			token += c;
			getChar();
			//如果读到末尾，跳出循环
			if (finish) 
			{
				break;
			}
		}
		//如果到末尾，不回退
		if (!finish) 
		{
			retract(); // 回退一个字符
		}
		// cout << token << endl;
		symbolCode = reserver();
		//Word word(token, code);
	}

	//判断数字常量
	else if (isDigit(c)) 
	{
		while (isDigit(c)) 
		{
			token += c;
			getChar();
			if (finish) 
			{
				break;
			}
		}
		if (!finish) 
		{
			retract();
		}
		value = atoi(token.c_str());
		// cout << x << endl;
		symbolCode = INTCON;
	}

	// +
	else if (isPlus(c)) 
	{
		symbolCode = PLUS;
		token = "+";
	}

	// -
	else if (isMinus(c)) 
	{
		symbolCode = MINU;
		token = "-";
	}

	// *
	else if (isStar(c)) 
	{
		symbolCode = MULT;
		token = "*";
	}

	// '/'
	// TODO: 注释？
	else if (isDivi(c)) 
	{
		symbolCode = DIV;
		token = "/";
	}

	// :
	else if (isColon(c)) 
	{
		symbolCode = COLON;
		token = ":";
	}

	// ;
	else if (isSemi(c)) 
	{
		symbolCode = SEMICN;
		token = ";";
	}

	// ,
	else if (isComma(c)) 
	{
		symbolCode = COMMA;
		token = ",";
	}

	else if (isLt(c)) 
	{
		// TODO: 错误处理？读完到末尾 好像不用？
		getChar();
		if (isEqu(c)) 
		{
			symbolCode = LEQ;
			token = "<=";
		}
		else 
		{
			retract();
			symbolCode = LSS;
			token = "<";
		}

	}

	else if (isGt(c)) 
	{
		getChar();
		if (isEqu(c)) 
		{
			symbolCode = GEQ;
			token = ">=";
		}
		else 
		{
			retract();
			symbolCode = GRE;
			token = ">";
		}
	}

	else if (isEqu(c)) 
	{
		getChar();
		if (isEqu(c)) 
		{
			symbolCode = EQL;
			token = "==";
		}
		else 
		{
			retract();
			symbolCode = ASSIGN;
			token = "=";
		}
	}

	else if (isExclamation(c)) 
	{
		getChar();
		if (isEqu(c)) 
		{
			symbolCode = NEQ;
			token = "!=";
		}
	}

	// TODO: 引号不匹配？？
	// 空字符？
	else if (isSingleq(c)) 
	{
		getChar();
		while (!isSingleq(c)) 
		{
			token += c;
			getChar();
		}
		symbolCode = CHARCON;
	}

	else if (isDoubleq(c)) 
	{
		getChar();
		while (!isDoubleq(c)) 
		{
			token += c;
			getChar();
		}
		symbolCode = STRCON;
	}

	// TODO: 括号匹配
	else if (isLpar(c)) 
	{
		symbolCode = LPARENT;
		token = "(";
	}
	else if (isRpar(c)) 
	{
		symbolCode = RPARENT;
		token = ")";
	}
	else if (isLbrack(c)) 
	{
		symbolCode = LBRACK;
		token = "[";
	}
	else if (isRbrack(c)) 
	{
		symbolCode = RBRACK;
		token = "]";
	}
	else if (isLbrace(c)) 
	{
		symbolCode = LBRACE;
		token = "{";
	}
	else if (isRbrace(c)) 
	{
		symbolCode = RBRACE;
		token = "}";
	}

	//如果读到结尾，返回
	if (finish) 
	{
		return false;
	}

	return true;
}

int LexicalAnalysis::getValue()
{
	return value;
}

bool LexicalAnalysis::isFinish()
{
	return finish;
}

void LexicalAnalysis::setAutoComplete()
{
	autoComplete = true;
}

int LexicalAnalysis::getIndex()
{
	return index;
}

void LexicalAnalysis::setIndex(int index)
{
	this->index = index;
}

void LexicalAnalysis::backup()
{
	backupIndex = index;
	backupToken = token;
	backupSymbolCode = symbolCode;
}

void LexicalAnalysis::restore()
{
	index = backupIndex;
	backupIndex = -1;
	token = backupToken;
	backupToken = "";
	symbolCode = backupSymbolCode;
}