#pragma once
#include "BlockStatementNode.h"
#include "StatementNode.h"
#include "ExpressionNode.h"
#include "VariableDeclarationNode.h"
#include <memory>
class ForLoopNode : public StatementNode {
	public:
		std::unique_ptr<VariableDeclarationNode> iterator;
		std::unique_ptr<ExpressionNode> condition;
		std::unique_ptr<ExpressionNode> operationsOnIterators;
		BlockStatementNode innerBody;

		ForLoopNode(TokenModel token, std::unique_ptr<VariableDeclarationNode> Iterator,
			std::unique_ptr<ExpressionNode> Condition, std::unique_ptr<ExpressionNode> operations,
			BlockStatementNode body) : StatementNode(token), iterator(std::move(Iterator)),
			condition(std::move(Condition)), operationsOnIterators(std::move(operations)), innerBody(body) {

		};
};
