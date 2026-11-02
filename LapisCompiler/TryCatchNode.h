#pragma once
#include "ExpressionNode.h"
#include "StatementNode.h"
#include "BlockStatementNode.h"
#include <memory>
class TryCatchNode : public StatementNode{
	public:
		std::unique_ptr<BlockStatementNode> tryBlock;
		std::unique_ptr<BlockStatementNode> catchBlock;
		std::unique_ptr<ExpressionNode> catchingCondition;
		std::unique_ptr<BlockStatementNode> finallyBlock;

		TryCatchNode(TokenModel token, std::unique_ptr<BlockStatementNode> Try, std::unique_ptr<BlockStatementNode> Catch,
			std::unique_ptr<ExpressionNode> caught, std::unique_ptr<BlockStatementNode> Finally) :
			StatementNode(token), tryBlock(std::move(Try)), catchBlock(std::move(Catch)), catchingCondition(std::move(caught)),
			finallyBlock(std::move(Finally)) {
		};
};
