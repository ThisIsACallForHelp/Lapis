#pragma once
#include <memory>
#include "StatementNode.h"
#include "VariableDeclarationNode.h"
#include "EnumMemberNode.h"
#include <vector>
class EnumDeclarationNode : public StatementNode {
public:
    TokenModel identifier;
    Token::Tokens accessModifier;
    std::string underlyingType;
    std::vector<std::unique_ptr<EnumMemberNode>> members;

    EnumDeclarationNode(TokenModel token, TokenModel id, std::vector<std::unique_ptr<EnumMemberNode>> enumMembers,
        Token::Tokens accModifier = Token::noneToken, std::string baseType = "int32")
        : StatementNode(token), identifier(id), accessModifier(accModifier), underlyingType(baseType),
        members(std::move(enumMembers))
    {
    };
};
