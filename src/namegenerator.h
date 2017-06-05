/* 
 * Name generator class
 */

#pragma once

#include <libconfig.h++>
#include <string>
#include <vector>

using namespace libconfig;
using namespace std;

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

		string generateName(bool male = true);
};
