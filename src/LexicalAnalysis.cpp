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

int LexicalAnalysis::getSymbolCode()
{
	return symbolCode;
}

string LexicalAnalysis::getToken()
{
	return token;
}

int LexicalAnalysis::getLine()
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
	//���token����ȡ��һ���ַ�
	token.clear();
	getChar();

	//�����հ׷�
	while (isspace(c)) 
	{
		if (isNewline(c)) 
		{
			line++;
		}
		getChar();
		//�ļ�ĩβȫ�ǿհ׷�������
	}

	//�ж��ǲ�����ĸ
	if (isLetter(c)) 
	{
		//���ַ�ƴ�ӳ��ַ���
		while (isLetter(c) || isDigit(c)) 
		{
			token += c;
			getChar();
			//�������ĩβ������ѭ��
			if (finish) 
			{
				break;
			}
		}
		//�����ĩβ��������
		if (!finish) 
		{
			retract(); // ����һ���ַ�
		}
		// cout << token << endl;
		symbolCode = reserver();
		//Word word(token, code);
	}

	//�ж����ֳ���
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
		// int x = atoi(token.c_str());
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
	// TODO: ע�ͣ�
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
		// TODO: ���������굽ĩβ �����ã�
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

	// TODO: ���Ų�ƥ�䣿��
	// ���ַ���
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

	// TODO: ����ƥ��
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

	if (!token.empty())
	{
		output.lexicalAnalysisOutput(symbolCode, token);
	}

	//���������β������
	if (finish) 
	{
		return false;
	}

	return true;
}


