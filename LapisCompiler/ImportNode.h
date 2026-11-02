#pragma once
#include "StatementNode.h"
class ImportNode : public StatementNode {
public:
	bool isLazy;
	std::string specifiedFile;
	std::string absoluteFilePath;
	ImportNode(TokenModel token, bool lazy, std::string linkedFile) :
		StatementNode(token), isLazy(lazy), specifiedFile(linkedFile) {

	};
};
