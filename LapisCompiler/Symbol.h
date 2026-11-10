#pragma once
#include <string>
#include "SymbolKind.h"
#include "Token.h"
#include "LifecycleState.h"
#include "TokenModel.h"
class Symbol {
	std::string name;
	std::string typeName;
	SymbolKind kind;
	Token::Tokens visibility;
	LifecycleState lifecycle;
	bool isConst;
	bool isReadonly;
	bool isDisposable;
	bool isHeap;
	bool isOptional;
	int ptrDepth;
	TokenModel definitionToken;


	public:
		Symbol(std::string Name, std::string Type,
			SymbolKind Kind, Token::Tokens Visibility,
			LifecycleState Lifecycle, TokenModel DefToken,
			bool IsConst = false, bool IsReadonly = false,
			bool IsDisposable = false, bool IsHeap = false, bool IsOptional = false,
			int PtrLVL = 0) :
			name(Name), typeName(Type), kind(Kind),
			visibility(Visibility), lifecycle(Lifecycle),
			definitionToken(DefToken), isConst(IsConst),
			isReadonly(IsReadonly), isDisposable(IsDisposable),
			isHeap(IsHeap), isOptional(IsOptional),
			ptrDepth(PtrLVL) {
		};

		inline std::string GetName() {
			return name; 
		}
		inline std::string GetType() {
			return typeName;
		}
		inline SymbolKind GetKind() {
			return kind;
		}
		inline Token::Tokens GetVisibility() {
			return visibility;
		}
		inline LifecycleState GetLifecycle() {
			return lifecycle;
		}
		inline bool GetConst() {
			return isConst;
		}
		inline bool GetReadonly() {
			return isReadonly;
		}
		inline bool GetDisposable() {
			return isDisposable;
		}
		inline bool GetIsHeap() {
			return isHeap;
		}
		inline bool GetOptional() {
			return isOptional;
		}
		inline int GetPtrDepth() {
			return ptrDepth;
		}
		inline TokenModel GetDefinition() {
			return definitionToken;
		}


		inline void SetName(std::string n) {
			name = n;
		}
		inline void SetType(std::string type) {
			typeName = type;
		}
		inline void SetKind(SymbolKind Kind) {
			kind = Kind;
		}
		inline void SetVisibility(Token::Tokens vis) {
			visibility = vis;
		}
		inline void SetLifecycle(LifecycleState Lifecycle) {
			lifecycle = Lifecycle;
		}
		inline void SetConst(bool Const) {
			isConst = Const;
		}
		inline void SetReadonly(bool Readonly) {
			isReadonly = Readonly;
		}
		inline void SetDisposable(bool Disposable) {
			isDisposable = Disposable;
		}
		inline void SetIsHeap(bool Heap) {
			isHeap = Heap;
		}
		inline void SetOptional(bool Optional) {
			isOptional = Optional;
		}
		inline void SetPtrDepth(int depth) {
			ptrDepth = depth;
		}
		inline void SetDefinition(TokenModel def) {
			definitionToken = def;
		}
};
