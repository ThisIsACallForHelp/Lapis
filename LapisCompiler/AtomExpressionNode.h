#pragma once
#include "ExpressionNode.h"
#include <memory>
#include <vector>
class AtomExpressionNode : public ExpressionNode {
	public: 
		int base;
		std::vector<std::unique_ptr<ExpressionNode>> args;

		AtomExpressionNode(TokenModel token,int Base, std::vector<std::unique_ptr<ExpressionNode>> params) :
			ExpressionNode(token), base(Base), args(std::move(params)) {
		};
};
