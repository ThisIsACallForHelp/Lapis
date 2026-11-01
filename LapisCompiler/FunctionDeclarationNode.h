#pragma once
#include "StatementNode.h"
#include "BlockStatementNode.h"
#include "ReturnStatementNode.h"
#include "VariableDeclarationNode.h"
#include <memory>
class FunctionDeclarationNode : public StatementNode {
	public:
		std::string functionName;
		std::unique_ptr<Token> accessModifier;
		std::unique_ptr<Token> specifier;
		std::unique_ptr<std::string> returnType;
		std::vector<std::unique_ptr<VariableDeclarationNode>> args;
		std::unique_ptr<BlockStatementNode> code;
		std::unique_ptr<ReturnStatementNode> returnedStatement;

		FunctionDeclarationNode(TokenModel token, std::string funcName, std::unique_ptr<Token> accModifier,
			std::unique_ptr<Token> Specifier, std::unique_ptr<std::string> typeReturned,
			std::unique_ptr<BlockStatementNode> innerBody, std::unique_ptr<ReturnStatementNode> retState,
			std::vector<std::unique_ptr<VariableDeclarationNode>> params) :
			StatementNode(token), functionName(funcName), accessModifier(std::move(accModifier)), specifier(std::move(Specifier)),
			returnType(std::move(typeReturned)), code(std::move(innerBody)), returnedStatement(std::move(retState)), args(std::move(params)) {

		};
};
