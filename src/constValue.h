#ifndef _CONST_VALUE_H_
#define _CONST_VALUE_H_

#define RESERVER_NUM 15
#define SYMBOL_NUM 39

enum SymbolCode
{
 IDENFR, 	// ��ʶ��
 INTCON, 		// ���γ���
 CHARCON, 		// �ַ�����
 STRCON,		// �ַ���
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

#endif // !_CONST_VALUE_H_
