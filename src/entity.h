#pragma once

#include <string>

class Entity {
	private:
		std::string entityName;
	public:
		void setName(std::string thename) { entityName = thename; };
		std::string getName() { return entityName; };
};
