#pragma once
#include <memory>
#include "ExpressionNode.h"
class AssignmentNode : public ExpressionNode {
	public:
		std::unique_ptr<ExpressionNode> target;
		Token::Tokens assignmentType;
		std::unique_ptr<ExpressionNode> value;

		AssignmentNode(TokenModel token, std::unique_ptr<ExpressionNode> Target, Token::Tokens AssignType,
			std::unique_ptr<ExpressionNode> val) :
			ExpressionNode(token), target(std::move(Target)), assignmentType(AssignType),
			value(std::move(val)) {
		};
};
