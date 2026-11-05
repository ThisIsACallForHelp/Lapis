#pragma once
#include "ExpressionNode.h"
#include <memory>
#include <vector>
class ReplaceExpressionNode : public ExpressionNode {
	public:
		std::vector<std::unique_ptr<ExpressionNode>> vals;
		std::unique_ptr<ExpressionNode> src;
		ReplaceExpressionNode(TokenModel token,
			std::vector<std::unique_ptr<ExpressionNode>> values,
			std::unique_ptr<ExpressionNode> source) :
			ExpressionNode(token), vals(std::move(values)),
			src(std::move(source)){
		};
};
