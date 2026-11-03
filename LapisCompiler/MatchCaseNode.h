#pragma once
#include "StatementNode.h"
#include "ExpressionNode.h"
#include "BlockStatementNode.h"
#include "ASTNode.h"
#include <memory>
class MatchCaseNode : public ASTNode {
	public:
		bool isDefault;
		std::unique_ptr<ExpressionNode> pattern;
		std::unique_ptr<StatementNode> body;
		std::unique_ptr<ExpressionNode> guard;

		MatchCaseNode(TokenModel token, bool isDef, std::unique_ptr<ExpressionNode> Pattern, std::unique_ptr<ExpressionNode> Guard,
			std::unique_ptr<StatementNode> innerBody) : ASTNode(token), isDefault(isDef), pattern(std::move(Pattern)),
			body(std::move(innerBody)), guard(std::move(Guard)) {
		};
};
