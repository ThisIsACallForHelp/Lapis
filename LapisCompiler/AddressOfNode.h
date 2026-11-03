#pragma once
#include <memory>
#include "ExpressionNode.h"
class AddressOfNode : public ExpressionNode {
public:
	std::string objectName;

	AddressOfNode(TokenModel token, std::string name)
		: ExpressionNode(token), objectName(name) {
	};
};
