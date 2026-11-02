#pragma once
#include <memory>
#include "StatementNode.h"
#include "VariableDeclarationNode.h"

class EnumDeclarationNode : public StatementNode {
	public:
		TokenModel identifier;
		Token::Tokens accessModifiers;
		std::string underlyingType;


};
