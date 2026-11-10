#pragma once
#include <unordered_map>
#include "Type.h"
class TypeSystem {
	std::unordered_map<std::string, Type> _registeredTypes;
	Type _type;

};
