#pragma once
#include "ExpressionNode.h"
#include <memory>
class BinaryExpressionNode : public ExpressionNode {
	public:
		std::unique_ptr<ExpressionNode> left;
		Token::Tokens operation;
		std::unique_ptr<ExpressionNode> right;

		BinaryExpressionNode(TokenModel token, std::unique_ptr<ExpressionNode> l, Token::Tokens op, std::unique_ptr<ExpressionNode> r)
			: ExpressionNode(token), left(std::move(l)), operation(op), right(std::move(r)) {
		};
};
