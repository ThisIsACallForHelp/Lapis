#pragma once
#include "StatementNode.h"
#include "ExpressionNode.h"
#include <memory>
#include <string>

class VariableDeclarationNode : public StatementNode {
    public:
        TokenModel Identifier;
        std::string TypeName;
        std::unique_ptr<ExpressionNode> Value;
        bool isConst;
        bool isReadonly;
        bool isHeapAllocated;
        bool isDisposable;
        bool isIsolated;
        bool isLazy;
        VariableDeclarationNode(TokenModel token, TokenModel id, std::string type, std::unique_ptr<ExpressionNode> val,
            bool isConstant = false, bool onHeap = false, bool disposable = false, bool readonly = false,
            bool isolated = false, bool lazy = false)
            : StatementNode(token), Identifier(id), TypeName(type), Value(std::move(val)), isConst(isConstant),
            isReadonly(readonly), isHeapAllocated(onHeap), isDisposable(disposable), isIsolated(isolated),
            isLazy(lazy)
        {

        };
};