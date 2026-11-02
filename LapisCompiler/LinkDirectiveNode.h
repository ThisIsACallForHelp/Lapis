#pragma once
#include "StatementNode.h"
class LinkDirectiveNode : public StatementNode {
	public:
		bool isLazy;
		std::string linkedFileName;
		LinkDirectiveNode(TokenModel token, bool lazy, std::string linkedFile) :
			StatementNode(token), isLazy(lazy), linkedFileName(linkedFile) {

		};
};
