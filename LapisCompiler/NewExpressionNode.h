#pragma once
#include <memory>
#include "ExpressionNode.h"
#include <vector>
class NewExpressionNode : public ExpressionNode {
	public:
		std::vector<std::unique_ptr<ExpressionNode>> args;
		std::string type;

		NewExpressionNode(TokenModel token, std::vector<std::unique_ptr<ExpressionNode>> params,
			std::string Type) : ExpressionNode(token), args(std::move(params)), type(Type) {
		};
};
