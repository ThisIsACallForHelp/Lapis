#pragma once
#include "ASTNode.h"
class LinkDirectiveNode : public ASTNode {
	public:
		bool isLazy;
		std::string linkedFileName;
		LinkDirectiveNode(TokenModel token, bool lazy, std::string linkedFile) :
			ASTNode(token), isLazy(lazy), linkedFileName(linkedFile) {

		};
};
