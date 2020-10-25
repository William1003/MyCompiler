#include"constValue.h"

const char* SymbolName[SYMBOL_NUM]
{
	"IDENFR", "INTCON", "CHARCON", "STRCON", "CONSTTK", "INTTK", "CHARTK", "VOIDTK",
	"MAINTK", "IFTK", "ELSETK", "SWITCHTK", "CASETK", "DEFAULTTK", "WHILETK", "FORTK",
	"SCANFTK", "PRINTFTK", "RETURNTK", "PLUS", "MINU", "MULT", "DIV", "LSS", "LEQ",
	"GRE", "GEQ", "EQL", "NEQ", "COLON", "ASSIGN", "SEMICN", "COMMA", "LPARENT",
	"RPARENT", "LBRACK", "RBRACK", "LBRACE", "RBRACE"
};

const char* ErrorContent[ERROR_NUM]
{
	"a",
	"b",
	"c",
	"d",
	"e",
	"f",
	"g",
	"h",
	"i",
	"j",
	"k",
	"l",
	"m",
	"n",
	"o",
	"p"
};

const char* ItemType[TYPE_NUM]
{
	"VOID", "INT", "CHAR"
};

const char* ItemKind[KIND_NUM]
{
	"CONST", "VAR", "ARRAY", "FUNCTION", "PARAMETER"
};