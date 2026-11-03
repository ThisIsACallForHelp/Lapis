#pragma once
#include <vector>
#include <memory>
#include "ExpressionNode.h"
class CastExpressionNode : public ExpressionNode {
	public:
		std::unique_ptr<ExpressionNode> arg;
		std::string type;

		CastExpressionNode(TokenModel token, std::unique_ptr<ExpressionNode> param,
			std::string destType) : ExpressionNode(token), arg(std::move(param)), type(destType) {
		};
};
