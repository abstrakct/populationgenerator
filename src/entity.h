#pragma once

#include <string>
#include "name.h"

class Entity {
	private:
		std::string entityName;
	public:
		void setName(std::string thename) { entityName = thename; };
		std::string getName() { return entityName; };
};

// vim: fdm=syntax ft=cpp
