#pragma once
#include "ExpressionNode.h"
#include <memory>
class MemberAccessNode : public ExpressionNode {
	public:
		TokenModel name;
		std::unique_ptr<ExpressionNode> value;

		MemberAccessNode(TokenModel token, TokenModel memberName,
			std::unique_ptr<ExpressionNode> Value) : ExpressionNode(token), 
			name(memberName), value(std::move(Value))
		{
		};
};

