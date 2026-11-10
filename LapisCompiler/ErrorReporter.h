#pragma once
#include <string>
#include "SymbolError.h"
#include <vector>
class ErrorReporter {
	std::string _sourceCode;
	std::string _fileName;
	std::vector<SymbolError> _errors;

};
