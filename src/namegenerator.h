/* 
 * Name generator class
 */

#pragma once

#include <libconfig.h++>
#include <string>
#include <vector>

using namespace libconfig;
using namespace std;

// TODO: add married name!
class Name {
    private:
        std::string given, family;
    public:
        void setGiven(std::string g) { given = g; };
        void setFamily(std::string f) { family = f; };
        std::string get() { return given + " " + family; };
        void set(string g, string f) { given = g; family = f; };
        void set(Name n) { given = n.given; family = n.family; };
};

class NameGenerator {
	private:
		int commonPercentage, numCommonFamilyNames, numUncommonFamilyNames, numMaleNames, numFemaleNames;
		std::vector<std::string> commonFamilyNames;
		std::vector<std::string> uncommonFamilyNames;
		std::vector<std::string> maleNames;
		std::vector<std::string> femaleNames;

		void readDataFiles();

	public:
		NameGenerator();
		~NameGenerator();

		Name generateName(bool male = true);
};
