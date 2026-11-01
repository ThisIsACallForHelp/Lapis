#pragma once
#include "Token.h"
#include <string>
class TokenModel
{
	int _lineNumber;
	int _charPosition;
	Token::Tokens _token;
	std::string _tokenValue;
	public:
		TokenModel(Token::Tokens token, std::string value,int  line, int position) :
		_token(token), _tokenValue(value), _lineNumber(line), _charPosition(position) 
		{}
		inline int getLineNumber() const { return _lineNumber; }
		inline int getCharPosition() const { return _charPosition; }
		inline Token::Tokens getTokenType() const { return _token; }
		inline std::string getTokenValue() const { return _tokenValue; }
};

