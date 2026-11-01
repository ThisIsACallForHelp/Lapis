#pragma once
#include "StatementNode.h"
#include "ExpressionNode.h"
#include "BlockStatementNode.h"
class WhileLoopNode : public StatementNode {
	public:
		ExpressionNode condition;
		BlockStatementNode innerBody;

		WhileLoopNode(TokenModel token, ExpressionNode conditionExp, BlockStatementNode blockState) :
			StatementNode(token), condition(conditionExp), innerBody(blockState) {
		};
};
