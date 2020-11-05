#ifndef _CONST_VALUE_H_
#define _CONST_VALUE_H_

#define RESERVER_NUM 15
#define SYMBOL_NUM 39
#define ERROR_NUM 16
#define TYPE_NUM 3
#define KIND_NUM 5

extern const char* SymbolName[SYMBOL_NUM];
extern const char* ErrorContent[ERROR_NUM];
extern const char* ItemType[TYPE_NUM];
extern const char* ItemKind[KIND_NUM];

enum SymbolCode
{
 IDENFR, 	// 标识符
 INTCON, 		// 整形常量
 CHARCON, 		// 字符常量
 STRCON,		// 字符串
 CONSTTK,		// const
 INTTK,			// int
 CHARTK,		// char
 VOIDTK,		// void
 MAINTK,		// main
 IFTK,		// if
 ELSETK,		// else
 SWICHTK,		// switch
 CASETK,		// case
 DEFAULTTK,	// default
 WHILETK,		// while
 FORTK,		// for
 SCANFTK,		// scanf
 PRINTFTK,		// printf
 RETURNTK,		// return
 PLUS,		// +
 MINU,			// -
 MULT,			// *
 DIV,			// /
 LSS,			// <
 LEQ,			// <=
 GRE,			// >
 GEQ,			// >=
 EQL,			// ==
 NEQ,			// !=
 COLON,		// :
 ASSIGN,		// =
 SEMICN,		// ;
 COMMA,		// ,
 LPARENT,		// (
 RPARENT,		// )
 LBRACK,		// [
 RBRACK,		// ]
 LBRACE,		// {
 RBRACE		// }
};

enum SymbolTableItemType
{
	VOID,
	INT,
	CHAR
};

enum SymbolTableItemKind
{
	CONST,
	VAR,
	ARRAY,
	FUNCTION,
	PARAMETER
};

enum ErrorCode
{
	A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P
};
#endif // !_CONST_VALUE_H_
