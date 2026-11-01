#pragma once
#include "Token.h"
#include "TokenModel.h"
#include <vector>
#include <string>

class Lexer
{
	std::string _codeString;
	int _lineNumber;
	int _charNumber;
	int _position;
public:
	Lexer(std::string code) : _codeString(code), _position(0) {
		_lineNumber = 1;
		_charNumber = 1;
	}
	char PeekNextChar(int step = 1);
	char NextChar();
	bool IsOutOfBoundary(int step = 1);
	TokenModel TokenizeText(std::string code);

};

