#include "Lexer.h"
#include "Token.h"
#include <map>
#include <stdexcept>

static std::map<std::string, Token> tokenDictionary;
char Lexer::PeekNextChar() {
	if (_position + 1 >= _codeString.size()) {
		throw std::out_of_range("the index is out of the string's range");
	}
	_position++;
	return _codeString[_position];
}

Token Lexer::TokenizeText(std::string code){

}
