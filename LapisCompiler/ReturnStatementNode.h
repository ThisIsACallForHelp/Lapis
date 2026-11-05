#pragma once
#include "StatementNode.h"
#include "ExpressionNode.h"
#include <memory>
#include <vector>

class ReturnStatementNode : public StatementNode {
public:
    std::vector<std::unique_ptr<ExpressionNode>> returnedVals;

    ReturnStatementNode(TokenModel token, std::vector<std::unique_ptr<ExpressionNode>> returned)
        : StatementNode(token), returnedVals(std::move(returned))
    {
    };
};