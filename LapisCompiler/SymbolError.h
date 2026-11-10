#pragma once
#include <string>
#include "TokenModel.h"
class SymbolError {
public:
	std::string errorStr;
	TokenModel token;
	std::string hint;

	public:
		SymbolError(std::string err, TokenModel Token,
			std::string Hint) :
			errorStr(err), token(Token), hint(Hint) {

		};

};
