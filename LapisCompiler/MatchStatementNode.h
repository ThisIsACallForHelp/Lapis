#pragma once
#include "StatementNode.h"
#include "ExpressionNode.h"
#include "MatchCaseNode.h"
#include <memory>
#include <vector>
class MatchStatementNode : public StatementNode {
	public:
		std::unique_ptr<ExpressionNode> target;
		std::vector<std::unique_ptr<MatchCaseNode>> cases;

		MatchStatementNode(TokenModel token, std::unique_ptr<ExpressionNode> Target, std::vector<std::unique_ptr<MatchCaseNode>> Cases)
			: StatementNode(token), target(std::move(Target)), cases(std::move(Cases)) {
		};
};
