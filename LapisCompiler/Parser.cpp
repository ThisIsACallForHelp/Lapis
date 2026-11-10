#include "Parser.h"
#include <stdexcept>
#include <format>
#include <sstream>
#include "AST.h"
TokenModel Parser::Current() const {
	return _tokens[_position];
}

bool IsAnOperand(Token::Tokens tokenType) {
	return (tokenType == Token::equalsToken
		|| tokenType == Token::plusEqualsToken
		|| tokenType == Token::minusEqualsToken
		|| tokenType == Token::astrixEqualsToken
		|| tokenType == Token::divisionEqualsToken
		|| tokenType == Token::caretEqualsToken);
}

bool IsAccessModifier(Token::Tokens token) {
	return (token == Token::publicToken ||
		token == Token::privateToken ||
		token == Token::protectedToken);
}

bool IsAnOptionalModifier(Token::Tokens token) {
	return (token == Token::staticToken ||
		token == Token::abstractToken ||
		token == Token::unsafeToken ||
		token == Token::sealedToken);
}

bool IsALogicalOperator(Token::Tokens type) {
	return (type == Token::pipePipeToken ||
		type == Token::ampersandAmpersandToken ||
		type == Token::caretToken ||
		type == Token::bitwiseShiftLeftToken ||
		type == Token::bitwiseShiftRightToken);
}

void Parser::MatchThisByTypeAndAdvance(Token::Tokens expected) {
	Token::Tokens tokenType = this->Current().GetTokenType();
	this->MatchByType(expected, tokenType);
	this->Advance();
}

TokenModel Parser::Advance() {
	if (!isOutOfBoundary()) {
		return _tokens[++_position];
	}
	return TokenModel(Token::eofToken, "", _tokens[_tokens.size() - 1].GetLineNumber(),
		_position);
}

bool Parser::isOutOfBoundary(int offset) const {
	return (_position + offset >= _tokens.size());
}

Token::Tokens Parser::MatchByType(Token::Tokens expected, Token::Tokens existing) {
	if (expected == existing) {
		return expected;
	}
	std::ostringstream oss;
	oss << "Parser Panic! Caught An Unexpected Token-Type At Line " << this->Current().GetLineNumber() 
		<< ", With An Input Type " << existing << ". Did You Mean " << expected <<"?";
	std::string exceptionStr = oss.str();
	throw std::runtime_error(exceptionStr);
}

TokenModel Parser::Match(TokenModel expected) const{
	TokenModel current = this->Current();
	if (expected == current) {
		return expected;
	}
	std::ostringstream oss;
	oss << "Parser Panic! Caught An Unexpected Token At Line " << current.GetLineNumber() <<
		   " At Char Number " << current.GetCharPosition() << ". Did You Mean " << expected.GetTokenType() <<"?";
	std::string exceptionStr = oss.str();
	throw std::runtime_error(exceptionStr);
}

TokenModel Parser::Peek(int offset) const {
	if (!isOutOfBoundary()) {
		return _tokens[_position + offset];
	}
	return TokenModel(Token::eofToken, "", _tokens[_tokens.size() - 1].GetLineNumber(),
		_position);
}

//std::unique_ptr<ProgramNode> Parser::Parse() {

//}

std::unique_ptr<FunctionDeclarationNode> Parser::ParseFuncDeclaration() {
	throw std::runtime_error("ParseFuncDeclaration not implemented");
}

std::unique_ptr<StatementNode> Parser::ParseStatementNode() {
	throw std::runtime_error("ParseStatementNode not implemented");
}

std::unique_ptr<VariableDeclarationNode> Parser::ParseVarDeclaration() {
	throw std::runtime_error("ParseVarDeclaration not implemented");
}

std::unique_ptr<AtomExpressionNode> Parser::ParseAtomExpression() {
	//atom<{base}>({vars})
	TokenModel astToken = this->Current();
	this->Advance();
	this->MatchThisByTypeAndAdvance(Token::openSharpArrow);// <
	this->Advance();
	this->MatchThisByTypeAndAdvance(Token::integerLiteralToken);// base
	std::string base = this->Current().GetTokenValue();
	this->Advance();
	this->MatchThisByTypeAndAdvance(Token::closeSharpArrow); //>
	this->MatchThisByTypeAndAdvance(Token::openParenthesisToken);
	std::vector<std::unique_ptr<ExpressionNode>> args;
	while (this->Current().GetTokenType() != Token::closeParenthesisToken) {
		if (this->Current().GetTokenType() == Token::commaToken) {
			this->Advance();
			continue;
		}
		this->MatchByType(Token::identifierToken, this->Current().GetTokenType());
		args.push_back(std::make_unique<ExpressionNode>(this->Current()));
		this->Advance();
	}
	this->Advance();
	return std::make_unique<AtomExpressionNode>(astToken, std::stoi(base), std::move(args));
}

std::unique_ptr<ExpressionNode> Parser::ParseExpressionNode() {
	return this->ParseAssignmentNode();
}

std::unique_ptr<AddressOfNode> Parser::ParseAddressOfNode() {
	TokenModel astToken = this->Current();
	this->Advance();
	this->MatchThisByTypeAndAdvance(Token::openParenthesisToken);
	this->MatchByType(Token::identifierToken, this->Current().GetTokenType());
	std::string variableName = this->Current().GetTokenValue();
	this->Advance();
	this->MatchThisByTypeAndAdvance(Token::closeParenthesisToken);
	return std::make_unique<AddressOfNode>(astToken, variableName);
}

std::unique_ptr<AssignmentNode> Parser::ParseAssignmentNode() {
	TokenModel astToken = this->Current();
	Token::Tokens assignmentType;
	std::unique_ptr<ExpressionNode> target = nullptr;
	this->Advance();
	if (IsAnOperand(this->Current().GetTokenType())) { // {operator} {operand}
		assignmentType = this->Current().GetTokenType();
		this->Advance();
		this->MatchByType(Token::identifierToken, this->Current().GetTokenType());
		target = this->ParseExpressionNode();
		this->Advance();
		return std::make_unique<AssignmentNode>(astToken, std::move(target), assignmentType, nullptr);
	}
	else if (this->Current().GetTokenType() == Token::identifierToken) { 
		// {operand} {operator} {optionalVal}
		target = this->ParseExpressionNode();
		this->Advance();
		if (!IsAnOperand(this->Current().GetTokenType())) {
			std::ostringstream oss;
			oss << "Parser Panic! At Line " << this->Current().GetLineNumber() << ", Assignment Failed Due To Incorrect Syntax. The Variable "
				<< this->Peek(-1).GetTokenValue() << " Was Parsed, But The Assignment Expression Failed To";
			std::string exceptionStr = oss.str();
			throw std::runtime_error(exceptionStr);
		}
		Token::Tokens tokenType = this->Current().GetTokenType();
		this->Advance();
		if (tokenType == Token::plusPlusToken || tokenType == Token::minusMinusToken) {
			return std::make_unique<AssignmentNode>(astToken, std::move(target), assignmentType, nullptr);
		}
		if (this->Current().GetTokenType() == Token::identifierToken) {
			std::unique_ptr<ExpressionNode> val = this->ParseExpressionNode();
			return std::make_unique<AssignmentNode>(astToken, std::move(target), assignmentType, std::move(val));
		}
	}
	std::ostringstream oss;
	oss << "Parser Panic! Couldn't Parse The Assignment At Line " << this->Current().GetLineNumber() << ", Please, Check Your Code For Syntax Errors";
	std::string exceptionStr = oss.str();
	throw std::runtime_error(exceptionStr);
}


std::unique_ptr<ExpressionNode> Parser::ParseLogicalOR() {
	std::unique_ptr<ExpressionNode> left = ParseLogicalAND();
	TokenModel currentToken = this->Current();
	Token::Tokens tokenType = currentToken.GetTokenType();
	while (tokenType == Token::pipePipeToken) {
		currentToken = this->Current();
		tokenType = currentToken.GetTokenType();
		this->Advance();
		std::unique_ptr<ExpressionNode> right = this->ParseLogicalAND();
		left = std::make_unique<BinaryExpressionNode>(currentToken, std::move(left), tokenType, std::move(right));
	}
	return left;
}

std::unique_ptr<ExpressionNode> Parser::ParseLogicalAND() {
	std::unique_ptr<ExpressionNode> left = ParseBitwiseOR();
	TokenModel currentToken = this->Current();
	Token::Tokens tokenType = currentToken.GetTokenType();
	while (tokenType == Token::ampersandAmpersandToken) {
		currentToken = this->Current();
		tokenType = currentToken.GetTokenType();
		this->Advance();
		std::unique_ptr<ExpressionNode> right = this->ParseBitwiseOR();
		left = std::make_unique<BinaryExpressionNode>(currentToken, std::move(left), tokenType, std::move(right));
	}
	return left;
}

std::unique_ptr<ExpressionNode> Parser::ParseBitwiseOR() {
	std::unique_ptr<ExpressionNode> left = ParseBitwiseXOR();
	TokenModel currentToken = this->Current();
	Token::Tokens tokenType = currentToken.GetTokenType();
	while (tokenType == Token::pipeToken) {
		currentToken = this->Current();
		tokenType = currentToken.GetTokenType();
		this->Advance();
		std::unique_ptr<ExpressionNode> right = this->ParseBitwiseXOR();
		left = std::make_unique<BinaryExpressionNode>(currentToken, std::move(left), tokenType, std::move(right));
	}
	return left;
}
std::unique_ptr<ExpressionNode> Parser::ParseBitwiseXOR() {
	std::unique_ptr<ExpressionNode> left = ParseBitwiseAND();
	TokenModel currentToken = this->Current();
	Token::Tokens tokenType = currentToken.GetTokenType();
	while (tokenType == Token::caretToken) {
		currentToken = this->Current();
		tokenType = currentToken.GetTokenType();
		this->Advance();
		std::unique_ptr<ExpressionNode> right = this->ParseBitwiseAND();
		left = std::make_unique<BinaryExpressionNode>(currentToken, std::move(left), tokenType, std::move(right));
	}
	return left;
}

std::unique_ptr<ExpressionNode> Parser::ParseBitwiseAND() {
	std::unique_ptr<ExpressionNode> left = ParseEquality();
	TokenModel currentToken = this->Current();
	Token::Tokens tokenType = currentToken.GetTokenType();
	while (tokenType == Token::ampersandToken) {
		currentToken = this->Current();
		tokenType = currentToken.GetTokenType();
		this->Advance();
		std::unique_ptr<ExpressionNode> right = this->ParseEquality();
		left = std::make_unique<BinaryExpressionNode>(currentToken, std::move(left), tokenType, std::move(right));
	}
	return left;
}

std::unique_ptr<ExpressionNode> Parser::ParseEquality() {
	std::unique_ptr<ExpressionNode> left = this->ParseComparison();
	TokenModel currentToken = this->Current();
	Token::Tokens tokenType = currentToken.GetTokenType();
	while (tokenType == Token::equalsEqualsToken
		|| tokenType == Token::bangEqualsToken
		|| tokenType == Token::equalsEqualsEqualsToken
		|| tokenType == Token::bangEqualsEqualsToken
		|| tokenType == Token::tildeEqualsEqualsToken
		|| tokenType == Token::bangTildeEqualsToken
		|| tokenType == Token::astrixEqualsEqualsToken
		|| tokenType == Token::bangAstrixEqualsToken) {
		currentToken = this->Current();
		tokenType = currentToken.GetTokenType();
		this->Advance();
		std::unique_ptr<ExpressionNode> right = this->ParseComparison();
		left = std::make_unique<BinaryExpressionNode>(currentToken, std::move(left), tokenType, std::move(right));
	}
	return left;
}

std::unique_ptr<ExpressionNode> Parser::ParseComparison() {
	std::unique_ptr<ExpressionNode> left = ParseBitwiseShift();
	TokenModel currentToken = this->Current();
	Token::Tokens tokenType = currentToken.GetTokenType();
	while (tokenType == Token::lessThanEqualsToken ||
		   tokenType == Token::biggerEqualsToken ||
		   tokenType == Token::lessThanToken ||
		   tokenType == Token::biggerThanToken) {
		currentToken = this->Current();
		tokenType = currentToken.GetTokenType();
		this->Advance();
		std::unique_ptr<ExpressionNode> right = this->ParseBitwiseShift();
		left = std::make_unique<BinaryExpressionNode>(currentToken, std::move(left), tokenType, std::move(right));
	}
	return left;
}

std::unique_ptr<ExpressionNode> Parser::ParseBitwiseShift() {
	std::unique_ptr<ExpressionNode> left = ParseAdditive();
	TokenModel currentToken = this->Current();
	Token::Tokens tokenType = currentToken.GetTokenType();
	while (tokenType == Token::bitwiseShiftLeftToken ||
		   tokenType == Token::bitwiseShiftRightToken) {
		currentToken = this->Current();
		tokenType = currentToken.GetTokenType();
		this->Advance();
		std::unique_ptr<ExpressionNode> right = this->ParseAdditive();
		left = std::make_unique<BinaryExpressionNode>(currentToken, std::move(left), tokenType, std::move(right));
	}
	return left;
}

std::unique_ptr<ExpressionNode> Parser::ParseAdditive() {
	std::unique_ptr<ExpressionNode> left = ParseMultiplicative();
	TokenModel currentToken = this->Current();
	Token::Tokens tokenType = currentToken.GetTokenType();
	while (tokenType == Token::plusToken ||
		tokenType == Token::minusToken) {
		currentToken = this->Current();
		tokenType = currentToken.GetTokenType();
		this->Advance();
		std::unique_ptr<ExpressionNode> right = this->ParseMultiplicative();
		left = std::make_unique<BinaryExpressionNode>(currentToken, std::move(left), tokenType, std::move(right));
	}
	return left;
}

std::unique_ptr<ExpressionNode> Parser::ParseMultiplicative() {
	std::unique_ptr<ExpressionNode> left = ParseUnary();
	TokenModel currentToken = this->Current();
	Token::Tokens tokenType = currentToken.GetTokenType();
	while (tokenType == Token::astrixToken ||
		tokenType == Token::divisionToken || tokenType == Token::moduleToken) {
		currentToken = this->Current();
		tokenType = currentToken.GetTokenType();
		this->Advance();
		std::unique_ptr<ExpressionNode> right = this->ParseMultiplicative();
		left = std::make_unique<BinaryExpressionNode>(currentToken, std::move(left), tokenType, std::move(right));
	}
	return left;
}

std::unique_ptr<ExpressionNode> Parser::ParseUnary() {
	std::unique_ptr<ExpressionNode> left = ParsePrimary();
	TokenModel currentToken = this->Current();
	Token::Tokens tokenType = currentToken.GetTokenType();
	while (tokenType== Token::bangToken ||
		tokenType == Token::minusToken ||
		tokenType == Token::plusToken ||
		tokenType == Token::tildeToken ||
		tokenType == Token::plusPlusToken ||
		tokenType == Token::minusMinusToken ||
		tokenType == Token::ampersandToken ||
		tokenType == Token::astrixToken) {
		currentToken = this->Current();
		this->Advance();
		auto operand = this->ParseUnary();
		return std::make_unique<BinaryExpressionNode>(currentToken, std::move(left), tokenType, std::move(operand));
	}
	return ParsePrimary();
}

std::unique_ptr<ExpressionNode> Parser::ParsePrimary() {
	TokenModel currentToken = this->Current();
	Token::Tokens type = currentToken.GetTokenType();
	std::unique_ptr<ExpressionNode> expressionPtr = nullptr;
	if (type == Token::integerLiteralToken
		|| type == Token::floatLiteralToken
		|| type == Token::stringLiteralToken
		|| type == Token::charLiteralToken
		|| type == Token::byteLiteralToken
		|| type == Token::trueToken
		|| type == Token::falseToken
		|| type == Token::nullToken)
	{
		Advance();
		expressionPtr = std::make_unique<LiteralNode>(currentToken, currentToken, currentToken.GetTokenValue());
	}
	else if (type == Token::identifierToken) {
		Advance();
		expressionPtr = std::make_unique<VariableAccessNode>(currentToken, currentToken);
	}
	else if (type == Token::openParenthesisToken) {
		Advance(); 
		expressionPtr = this->ParseExpressionNode();
		MatchThisByTypeAndAdvance(Token::closeParenthesisToken);
	}
	else if (type == Token::addressToken) {
		expressionPtr = ParseAddressOfNode();
	}
	else {
		throw std::runtime_error("Parser Panic! Unexpected token '" + currentToken.GetTokenValue()
			+ "' when expecting an expression at line " + std::to_string(currentToken.GetLineNumber()));
	}
	while (true) {
		if (Current().GetTokenType() == Token::dotToken) {
			Advance();
			TokenModel member = Current();
			Advance();
			expressionPtr = std::make_unique<MemberAccessNode>(currentToken, member, std::move(expressionPtr));
		}
		else if (Current().GetTokenType() == Token::openBracketsToken) {
			Advance();
			std::unique_ptr<ExpressionNode> index = this->ParseExpressionNode();
			MatchThisByTypeAndAdvance(Token::closeBracketsToken); // consume ']'
			expressionPtr = std::make_unique<IndexAccessNode>(currentToken, std::move(expressionPtr), std::move(index));
		}
		else if (Current().GetTokenType() == Token::openParenthesisToken) {
			break;
		}
		else {
			break;
		}
	}
	return expressionPtr;
}

std::unique_ptr<BlockStatementNode> Parser::ParseBlockStatement() {
	std::vector<std::unique_ptr<StatementNode>> commands;
	TokenModel astToken = this->Current();
	this->MatchThisByTypeAndAdvance(Token::openBraceToken);
	while (this->Current().GetTokenType() != Token::closeBraceToken) {
		std::unique_ptr<StatementNode> statementPtr = this->ParseStatementNode();
		commands.push_back(std::move(statementPtr));
	}
	auto block = std::make_unique<BlockStatementNode>(astToken);
	block->statements = std::move(commands);
	return block;
}

std::unique_ptr<BreakStatementNode> Parser::ParseBreakStatement() {
	TokenModel astToken = this->Current();
	return std::make_unique<BreakStatementNode>(astToken);
}

std::unique_ptr<CastExpressionNode> Parser::ParseCastExpression() {
	this->MatchThisByTypeAndAdvance(Token::openParenthesisToken);
	TokenModel astToken = this->Current();
	this->MatchThisByTypeAndAdvance(Token::closeParenthesisToken);
	std::unique_ptr<ExpressionNode> destination = this->ParseExpressionNode();
	return std::make_unique<CastExpressionNode>(astToken, std::move(destination), astToken.GetTokenValue());
}

std::unique_ptr<ClassDeclarationNode> Parser::ParseClassDeclaration() {
	Token::Tokens accessModifier = Token::internalToken;
	Token::Tokens optionalModifier = Token::noneToken;
	Token::Tokens CurrTokenType = this->Current().GetTokenType();
	if (IsAccessModifier(CurrTokenType)) {
		accessModifier = CurrTokenType;
		CurrTokenType = this->Advance().GetTokenType();
	}
	if (IsAnOptionalModifier(CurrTokenType)) {
		optionalModifier = CurrTokenType;
		CurrTokenType = this->Advance().GetTokenType();
	}
	this->MatchThisByTypeAndAdvance(Token::classToken);
	CurrTokenType = this->Current().GetTokenType();
	TokenModel astToken = this->Current();
	TokenModel nameToken = this->Current();
	this->MatchByType(Token::identifierToken, nameToken.GetTokenType());
	std::string className = nameToken.GetTokenValue();
	this->Advance();
	std::vector<TokenModel> fatherClasses;
	if (this->Current().GetTokenType() == Token::extendsToken) {
		this->MatchThisByTypeAndAdvance(Token::extendsToken);
		TokenModel currToken = this->Current();
		while (currToken.GetTokenType() != Token::openBraceToken) {
			currToken = this->Current();
			if (currToken.GetTokenType() == Token::commaToken) {
				currToken = this->Advance();
				continue;
			}
			this->MatchByType(Token::identifierToken, currToken.GetTokenType());
			fatherClasses.push_back(currToken);
			this->Advance();
		}
		if (fatherClasses.size() == 0) {
			std::ostringstream oss;
			oss << "Parser Panic! In Declaration Of Class " << className <<
				". The Syntax Indicates Inheritance, Though No Father Class Was Mentioned At Line "
				<< currToken.GetLineNumber();
			std::string exceptionStr = oss.str();
			throw std::runtime_error(exceptionStr);
		}
	}
	
	this->MatchThisByTypeAndAdvance(Token::openBraceToken); 
	std::vector<std::unique_ptr<VariableDeclarationNode>> classFields;
	std::vector<std::unique_ptr<FunctionDeclarationNode>> classFuncs;
	CurrTokenType = this->Current().GetTokenType();
	while (this->Current().GetTokenType() != Token::closeBraceToken) {
		Token::Tokens accessModifier = Token::internalToken;
		if (IsAccessModifier(this->Current().GetTokenType())) {
			accessModifier = this->Current().GetTokenType();
			this->Advance();
			this->MatchThisByTypeAndAdvance(Token::openBraceToken);
			while (this->Current().GetTokenType() != Token::closeBraceToken) {
				Token::Tokens type = this->Current().GetTokenType();
				if (type == Token::functionToken || (IsAnOptionalModifier(type) && this->Peek(1).GetTokenType()
					== Token::functionToken)) {
					std::unique_ptr<FunctionDeclarationNode> funcDeclaration = this->ParseFuncDeclaration();
					funcDeclaration->accessModifier = accessModifier;
					classFuncs.push_back(std::move(funcDeclaration));
				}
				else {
					std::unique_ptr<VariableDeclarationNode> varDeclaration = this->ParseVarDeclaration();
					varDeclaration->accessModifier = accessModifier;
					classFields.push_back(std::move(varDeclaration));
				}
			}
			this->MatchThisByTypeAndAdvance(Token::closeBraceToken);
		}

		else {
			Token::Tokens type = this->Current().GetTokenType();

			if (type == Token::functionToken || (IsAnOptionalModifier(type) && this->Peek(1).GetTokenType() ==
				Token::functionToken)) {
				std::unique_ptr<FunctionDeclarationNode> funcDeclaration = this->ParseFuncDeclaration();
				funcDeclaration->accessModifier = accessModifier;
				classFuncs.push_back(std::move(funcDeclaration));
			}
			else {
				std::unique_ptr<VariableDeclarationNode> varDeclaration = this->ParseVarDeclaration();
				varDeclaration->accessModifier = accessModifier;
				classFields.push_back(std::move(varDeclaration));
			}
		}
	}
	this->MatchThisByTypeAndAdvance(Token::closeBraceToken);
	return std::make_unique<ClassDeclarationNode>(astToken, className, std::move(fatherClasses),
							std::move(classFields), std::move(classFuncs), optionalModifier, accessModifier);
}
