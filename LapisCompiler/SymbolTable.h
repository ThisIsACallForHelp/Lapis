#pragma once
#include <unordered_map>
#include "Symbol.h"
class SymbolTable {
	std::unordered_map<std::string, Symbol> _symbols;
	SymbolTable* _parent;
	int _scopeLevel;
	public:
		SymbolTable(std::unordered_map<std::string, Symbol> symbols = {},
			SymbolTable* parent = nullptr, int scopeLVL = 0) :
			_symbols(symbols), _parent(parent), _scopeLevel(scopeLVL) {
		};

		void Declare(Symbol symbol);
		Symbol* Lookup(std::string name);
		bool ExistsLocally(std::string name);
		SymbolTable EnterScope();
		void ExitScope();
		int GetScopeLevel();
};
