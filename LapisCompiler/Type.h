#pragma once
#include <string>
#include <vector>
class Type {
	std::string baseName;
	bool isOptional;
	int ptrDepth;
	std::vector<std::string> unionTypes;
	bool isPrimitive;
	public:
		Type(std::string name, std::vector<std::string> types,
			bool Optional = false, int Depth = 0,
			bool Primitive = false) :
			baseName(name), unionTypes(types), isOptional(Optional),
			ptrDepth(Depth), isPrimitive(Primitive) {
		};
		Type(std::string name, bool Primitive = false) :
			baseName(name), isPrimitive(Primitive) {

		};
		Type() : baseName(""), isOptional(false), ptrDepth(0),
			unionTypes({}), isPrimitive(false) {

		};
		std::string GetName() {
			return baseName;
		}
		bool GetOptional() {
			return isOptional;
		}
		int GetPtrDepth() {
			return ptrDepth;
		}
		std::vector<std::string> GetTypes() {
			return unionTypes;
		}
		bool GetPrimitive() {
			return isPrimitive;
		}

		inline void SetName(std::string n) {
			baseName = n;
		}
		inline void SetOptional(bool Opt) {
			isOptional = Opt;
		}
		inline void SetPtrDepth(int depth) {
			ptrDepth = depth;
		}
		inline void SetTypes(std::vector<std::string> types) {
			unionTypes = types;
		}
		inline void SetPrimitive(bool Primitive) {
			isPrimitive = Primitive;
		}
		inline bool operator==(Type& other)  {
			return this->baseName == other.GetName() &&
				this->isOptional == other.GetOptional() &&
				this->isPrimitive == other.GetPrimitive() &&
				this->ptrDepth == other.GetPtrDepth() &&
				this->unionTypes == other.GetTypes();
		}
		inline bool operator!=(Type& other) {
			return !(*this == other);
		}
};
