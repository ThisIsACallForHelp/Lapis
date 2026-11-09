#include "Lexer.h"
#include "Token.h"
#include "TokenModel.h"
#include <map>
#include <stdexcept>

static std::map<std::string, Token::Tokens> tokenDictionary = {
	{ "let", Token::letToken },
	{ "for", Token::forToken },
	{ "while", Token::whileToken },
	{ "until", Token::untilToken },
	{ "async", Token::asyncToken },
	{ "await", Token::awaitToken },
	{ "asm", Token::asmToken },
	{ "string", Token::stringToken },
	{ "char", Token::charToken },
	{ "int", Token::intToken },
	{ "double", Token::doubleToken },
	{ "float", Token::floatToken },
	{ "function", Token::functionToken },
	{ "if", Token::ifToken },
	{ "else", Token::elseToken },
	{ "match", Token::matchToken },
	{ "case", Token::caseToken },
	{ "continue", Token::continueToken },
	{ "break", Token::breakToken },
	{ "return", Token::returnToken },
	{ "void", Token::voidToken },
	{ "int8", Token::int8Token },
	{ "int16", Token::int16Token },
	{ "int32", Token::int32Token },
	{ "int64", Token::int64Token },
	{ "int128", Token::int128Token },
	{ "uint8", Token::uint8Token },
	{ "uint16", Token::uint16Token },
	{ "uint32", Token::uint32Token },
	{ "uint64", Token::uint64Token },
	{ "uint128", Token::uint128Token },
	{ "unsafe", Token::unsafeToken },
	{ "extern", Token::externToken },
	{ "import", Token::importToken },
	{ "using", Token::usingToken },
	{ "null", Token::nullToken },
	{ "is", Token::isToken },
	{ "module", Token::moduleToken },
	{ "move", Token::moveToken },
	{ "override", Token::overrideToken },
	{ "virtual", Token::virtualToken },
	{ "interface", Token::interfaceToken },
	{ "inherits", Token::inheritsToken },
	{ "abstract", Token::abstractToken },
	{ "enum", Token::enumToken },
	{ "type", Token::typeToken },
	{ "const", Token::constToken },
	{ "operator", Token::operatorToken },
	{ "true", Token::trueToken },
	{ "false", Token::falseToken },
	{ "this", Token::thisToken },
	{ "unfinished", Token::unfinishedToken },
	{"partial", Token::partialToken},
	{"allocate", Token::allocateToken},
	{ "new", Token::newToken },
	{ "class", Token::classToken },
	{ "public", Token::publicToken },
	{ "private", Token::privateToken },
	{ "protected", Token::protectedToken },
	{ "readonly", Token::readonlyToken },
	{ "heap", Token::heapToken },
	{ "arena", Token::arenaToken },
	{ "defer", Token::deferToken },
	{ "disposable", Token::disposableToken },
	{ "erase", Token::eraseToken },
	{ "isolated", Token::isolatedToken },
	{ "nocopy", Token::nocopyToken },
	{ "default", Token::defaultToken },
	{ "replace", Token::replaceToken },
	{ "radioactive", Token::radioactiveToken },
	{ "crash", Token::crashToken },
	{ "nuclear", Token::nuclearToken },
	{ "try", Token::tryToken },
	{ "catch", Token::catchToken },
	{ "finally", Token::finallyToken },
	{ "atom", Token::atomToken },
	{ "fusion", Token::fusionToken },
	{ "address", Token::addressToken },
	{ "readfrom", Token::readfromToken },
	{ "writeto", Token::writetoToken },
	{ "comptime", Token::comptimeToken },
	{ "lazy", Token::lazyToken },
	{ "distribute", Token::distributeToken },
	{ "inline", Token::inlineToken },
	{ "simd", Token::simdToken },
	{ "as", Token::asToken },
	{ "mut", Token::mutToken },
	{ "byte", Token::byteToken },
	{ "skeleton", Token::skeletonToken },
	{ "slot", Token::slotToken },
	{ "fast", Token::fastToken },
	{ "macro", Token::macroToken },
	{ "extends", Token::extendsToken },
	{ "memoized", Token::memoizedToken },
	{ "yield", Token::yieldToken }
};

static std::map<std::string, Token::Tokens> singleCharOperators = {
	{ "=",  Token::equalsToken },
	{ "+",  Token::plusToken },
	{ "-",  Token::minusToken },
	{ "*",  Token::astrixToken },
	{ "/",  Token::divisionToken },
	{ "%",  Token::percentToken },
	{ "&",  Token::ampersandToken },
	{ "|",  Token::pipeToken },
	{ "^",  Token::caretToken },
	{ "~",  Token::tildeToken },
	{ "!",  Token::bangToken },
	{ "<",  Token::lessThanToken },
	{ ">",  Token::biggerThanToken },
	{ "?",  Token::questionMarkToken },
	{ "#",  Token::hashToken },
	{ "(",  Token::openParenthesisToken },
	{ ")",  Token::closeParenthesisToken },
	{ "[",  Token::openBracketsToken },
	{ "]",  Token::closeBracketsToken },
	{ "{",  Token::openBraceToken },
	{ "}",  Token::closeBraceToken },
	{ ";",  Token::semicolonToken },
	{ ":",  Token::colonToken },
	{ ",",  Token::commaToken },
	{ ".",  Token::dotToken },
	{ "\\", Token::backwardsSlashToken },
	{ "\"", Token::doubleQuotesToken }
};

static std::map<std::string, Token::Tokens> multiCharOperators = {
	{ "===", Token::equalsEqualsEqualsToken },
	{ "!==", Token::bangEqualsEqualsToken },
	{ "~==", Token::tildeEqualsEqualsToken },
	{ "!~=", Token::bangTildeEqualsToken },
	{ "!*=", Token::bangAstrixEqualsToken },
	{ "*==", Token::astrixEqualsEqualsToken },
	{ "==",  Token::equalsEqualsToken },
	{ "!=",  Token::bangEqualsToken },
	{ "+=",  Token::plusEqualsToken },
	{ "-=",  Token::minusEqualsToken },
	{ "*=",  Token::astrixEqualsToken },
	{ "/=",  Token::divisionEqualsToken },
	{ "^=",  Token::caretEqualsToken },
	{ "++",  Token::plusPlusToken },
	{ "--",  Token::minusMinusToken },
	{ "&&",  Token::ampersandAmpersandToken },
	{ "||",  Token::pipePipeToken },
	{ "<<",  Token::bitwiseShiftLeftToken },
	{ ">>",  Token::bitwiseShiftRightToken },
	{ "<=",  Token::lessThanEqualsToken },
	{ ">=",  Token::biggerEqualsToken },
	{ "=>",  Token::fatArrowToken },
	{ "->",  Token::arrowToken },
	{ "..",  Token::rangeToken },
	{ "//",  Token::slashSlashToken }
};



bool Lexer::IsOutOfBoundary(int step) {
	if (_position + step >= _codeString.size()) {
		return true;
	}
	return false;
}



char Lexer::PeekNextChar(int step) {
	return (IsOutOfBoundary(step)) ? Token::eofToken : _codeString[_position + step];
}

char Lexer::NextChar() {
	_position++;
	return (IsOutOfBoundary()) ? EOF : _codeString[_position];
}

bool IsASingleOperator(std::string character){
	return (singleCharOperators.find(character) != singleCharOperators.end());
}

bool IsLogicalOperator(std::string character) {
	return (multiCharOperators.find(character) != multiCharOperators.end());
}


TokenModel Lexer::TokenizeText(std::string code){
	if (_position >= code.size()) {
		return TokenModel(Token::eofToken, "", _lineNumber, _charNumber - 1);
	}
	char currentChar = code[_position];
	std::vector<char> distinctOperators = {
		'&', '|', '^', '+', '-', '*', '/', '.', '=', '>',
		'<', '~', '!'
	};
	std::map<char, Token::Tokens> punctuations = {
		{'(', Token::openParenthesisToken},
		{')', Token::closeParenthesisToken},
		{'[', Token::openBracketsToken},
		{']', Token::closeBracketsToken},
		{'{', Token::openBraceToken},
		{'}', Token::closeBraceToken},
		{';', Token::semicolonToken},
		{',', Token::commaToken},
		{':', Token::colonToken}
	};
	if (currentChar == '\'') {
		currentChar = NextChar();
		if (PeekNextChar() != '\'') {
			return TokenModel(Token::badToken, std::string(1,currentChar), _lineNumber, _charNumber + 1);
		}
		char tokenVal = currentChar;
		currentChar = NextChar();
		return TokenModel(Token::charLiteralToken, std::string(1, tokenVal), _lineNumber, _charNumber - 2);
	}
	if (currentChar == '/' && PeekNextChar() == '/') {
		while (currentChar != '\n' && currentChar != EOF) {
			currentChar = NextChar();
			_charNumber++;
		}
		return TokenizeText(code);
	}
	else if (currentChar == '\"') {
		std::string stringContent = "";
		currentChar = NextChar();
		_charNumber++;
		while (currentChar != '\"' && currentChar != EOF) {
			stringContent += currentChar;
			currentChar = NextChar();
			_charNumber++;
		}
		return TokenModel(Token::stringLiteralToken, stringContent, _lineNumber, _charNumber - stringContent.size());
	}
	else if (currentChar == '\n') {
		this->_lineNumber++;
		this->_charNumber = 0;
		_position++;
		return TokenizeText(code);
	}
	else if (currentChar == ' ' || currentChar == '\r' || currentChar == '\t') {
		this->_charNumber++;
		_position++;
		return TokenizeText(code);
	}
	else if (currentChar == '#') {
		_position++;
		this->_charNumber++;
		return TokenModel(Token::hashToken, std::string(1,currentChar), this->_lineNumber, _charNumber - 1);
	}
	else if (isalpha(currentChar)) {
		std::string tokenWord = "";
		while (isalnum(currentChar) || currentChar == '_') {
			tokenWord += currentChar;
			currentChar = NextChar();
			this->_charNumber++;
		}
		if (tokenWord.compare("constructor") == 0) {
			return TokenModel(Token::functionToken, tokenWord, this->_lineNumber, this->_charNumber - tokenWord.size());
		}
		auto found = tokenDictionary.find(tokenWord);
		if (found != tokenDictionary.end()) {
			return TokenModel(found->second, tokenWord, _lineNumber, _charNumber - tokenWord.size());
		}
		return TokenModel(Token::identifierToken, tokenWord, this->_lineNumber, int(this->_charNumber - tokenWord.size()));
	}
	else if (isdigit(currentChar)) {
		std::string numberStr = "";
		while (isdigit(currentChar) && currentChar != '.') {
			numberStr += currentChar;
			currentChar = NextChar();
			_charNumber++;
		}
		if (currentChar == '.' && isdigit(PeekNextChar())) {
			_charNumber++;
			numberStr += '.';
			currentChar = NextChar();
			while (isdigit(currentChar)) {
				numberStr += currentChar;
				currentChar = NextChar();
				_charNumber++;
			}
			return TokenModel(Token::floatLiteralToken, numberStr, _lineNumber, _charNumber - numberStr.size());
		}
		return TokenModel(Token::integerLiteralToken, numberStr, _lineNumber, _charNumber - numberStr.size());
	}
	auto punc = punctuations.find(currentChar);
	if (punc != punctuations.end()) {
		_charNumber++;
		_position++;
		return TokenModel(punc->second, std::string(1, currentChar), _lineNumber, _charNumber);
	}
	else if (std::find(distinctOperators.begin(), distinctOperators.end(), currentChar) != distinctOperators.end()) {
		std::string tokenWord = std::string(1,currentChar) + PeekNextChar() + PeekNextChar(2);
		auto findOperator = multiCharOperators.find(tokenWord);
		if (findOperator == multiCharOperators.end()) {
			tokenWord = std::string(1, currentChar) + PeekNextChar();
			findOperator = multiCharOperators.find(tokenWord);
			if (findOperator == multiCharOperators.end()) {
				_position++;
				_charNumber++;
				return TokenModel(singleCharOperators[std::string(1, currentChar)], std::string(1, currentChar), _lineNumber, _charNumber - 1);
			}
		}
		_position += tokenWord.size();
		_charNumber += tokenWord.size();
		return TokenModel(findOperator->second, tokenWord, _lineNumber, _charNumber - tokenWord.size());
	}
	return TokenModel(Token::badToken,"", this->_lineNumber, this->_charNumber);
}
