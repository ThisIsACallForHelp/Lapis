#pragma once
#include "StatementNode.h"
#include "BlockStatementNode.h"
#include "ReturnStatementNode.h"
#include "VariableDeclarationNode.h"
#include <memory>
class FunctionDeclarationNode : public StatementNode {
	public:
		std::string functionName;
		Token::Tokens accessModifier;
		Token::Tokens specifier;
		std::string returnType;
		std::vector<std::unique_ptr<VariableDeclarationNode>> args;
		std::unique_ptr<BlockStatementNode> code;
		FunctionDeclarationNode(TokenModel token, std::string funcName,
			Token::Tokens accModifier,
			Token::Tokens Specifier,
			std::string typeReturned,
			std::unique_ptr<BlockStatementNode> innerBody,
			std::vector<std::unique_ptr<VariableDeclarationNode>> params) :
			StatementNode(token), functionName(funcName), accessModifier(accModifier),
			specifier(Specifier), returnType(typeReturned), code(std::move(innerBody)),
			args(std::move(params)) {
		};
};
