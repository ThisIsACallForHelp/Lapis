#pragma once
#include "ExpressionNode.h"
#include <memory>
class UnaryExpressionNode : public ExpressionNode {
	public:
		Token::Tokens unaryOperator;
		std::unique_ptr<ExpressionNode> Identifier;
		UnaryExpressionNode(TokenModel token, Token::Tokens unaryOp, std::unique_ptr<ExpressionNode> ID) :
			ExpressionNode(token), unaryOperator(unaryOp), Identifier(std::move(ID)) {
		};
};
