#pragma once
#include <unordered_map>
#include "SymbolTable.h"
class SafetyTracker {
	SymbolTable* _currTable;
	std::unordered_map<std::string, Symbol> _activeBorrows;

	public:
		SafetyTracker(SymbolTable& curr) :
			_currTable(&curr), _activeBorrows({}) {

		};
		void TrackVariable(std::string name, LifecycleState initial);
		void MarkErased(std::string name, TokenModel token);
		void MarkConsumed(std::string name, TokenModel location);
		void VerifyCanRead(std::string name, TokenModel location);
		void VerifyCanWrite(std::string name, TokenModel location);
		inline SymbolTable* GetCurrTable() {
			return _currTable;
		}
		inline std::unordered_map<std::string, Symbol> GetActiveBorrows() {
			return _activeBorrows;
		}
		inline void SetCurrTable(SymbolTable* table) {
			_currTable = table;
		}
		inline void SetActiveBorrows(std::unordered_map<std::string, Symbol> borrows) {
			_activeBorrows = borrows;
		}

};