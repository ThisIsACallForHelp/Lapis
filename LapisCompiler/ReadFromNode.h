#pragma once
#include <memory>
#include "ExpressionNode.h"
class ReadFromNode : public ExpressionNode {
	public:
		std::unique_ptr<ExpressionNode> address;

		ReadFromNode(TokenModel token, std::unique_ptr<ExpressionNode> Address)
			: ExpressionNode(token), address(std::move(Address)) {
		};
};
