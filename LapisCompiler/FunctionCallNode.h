#pragma once
#include "ExpressionNode.h"
#include <vector>
#include <memory>
class FunctionCallNode : public ExpressionNode {
	public:
		std::string functionName;
		std::vector<std::unique_ptr<ExpressionNode>> args;

		FunctionCallNode(TokenModel token, std::string funcName, std::vector<std::unique_ptr<ExpressionNode>> params) :
			ExpressionNode(token), functionName(funcName), args(std::move(params)) {
		};
};
