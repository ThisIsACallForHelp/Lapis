#pragma once
#include <memory>
#include "ExpressionNode.h"
class WriteToNode : public ExpressionNode {
public:
	std::unique_ptr<ExpressionNode> address;
	std::unique_ptr<ExpressionNode> value;
	WriteToNode(TokenModel token, std::unique_ptr<ExpressionNode> Address,
		std::unique_ptr<ExpressionNode> val): ExpressionNode(token), address(std::move(Address)),
		value(std::move(val)){
	};
};

