#pragma once

#include "entity.h"

class Person : public Entity {
	private:
		int birthYear;
	public:
		void setBirthYear(int y) { birthYear = y; };
		int  getBirthYear() { return birthYear; };
};
