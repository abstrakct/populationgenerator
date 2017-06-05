/*
 * Name Generator class
 */

#include <libconfig.h++>
#include <iostream>

#include "debug.h"
#include "utils.h"
#include "namegenerator.h"

NameGenerator::NameGenerator()
{
	readDataFiles();
}

NameGenerator::~NameGenerator()
{
}

void NameGenerator::readDataFiles()
{
	Config *cf = new Config();


    try {
        cf->readFile("data/config.cfg");
    } catch(const FileIOException &fioex) {
        std::cerr << "Couldn't read config file " << std::endl;
    } catch(const ParseException &pex) {
        std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
            << " - " << pex.getError() << std::endl;
    }

    const Setting& root = cf->getRoot();
    try {
        const Setting &nameoptions = root["nameoptions"][0];
        int count = nameoptions.getLength();
        dbg("Found %d name options.", count);
        nameoptions.lookupValue("common_percentage", commonPercentage);
    } catch(const SettingNotFoundException &nfex) {
        cout << "caught exception in parse_names" << endl;
    }

    try {
        int count;
        std::string path, result;

        // Read family names
        const Setting &familyname = cf->lookup("names.commonfamilynames");
        count = numCommonFamilyNames = familyname.getLength();
        dbg("Found %d common family names.", count);
        for(int i = 0; i < count; i++) {
            path = "names.commonfamilynames.[" + std::to_string(i) + "]";
            cf->lookupValue(path, result);
            commonFamilyNames.push_back(result);
        }

        const Setting &uncommonfamilyname = cf->lookup("names.uncommonfamilynames");
        count = numUncommonFamilyNames = uncommonfamilyname.getLength();
        dbg("Found %d uncommon family names.", count);
        for(int i = 0; i < count; i++) {
            path = "names.uncommonfamilynames.[" + std::to_string(i) + "]";
            cf->lookupValue(path, result);
            uncommonFamilyNames.push_back(result);
        }

        // Read male given names
        const Setting &malename = cf->lookup("names.malegivennames");
        count = numMaleNames = malename.getLength();
        dbg("Found %d male names.", count);
        for(int i = 0; i < count; i++) {
            path = "names.malegivennames.[" + std::to_string(i) + "]";
            cf->lookupValue(path, result);
            maleNames.push_back(result);
        }

        // Read female given names
        const Setting &femalename = cf->lookup("names.femalegivennames");
        count = numFemaleNames = femalename.getLength();
        dbg("Found %d female names.", count);
        for(int i = 0; i < count; i++) {
            path = "names.femalegivennames.[" + std::to_string(i) + "]";
            cf->lookupValue(path, result);
            femaleNames.push_back(result);
        }

    } catch(const SettingNotFoundException &nfex) {
        cout << "caught exception in parseNames" << endl;
    }
}

string NameGenerator::generateName(bool male)
{
    int i, j;
    string givenname, lastname, fullname;

    if(male) {
        i = ri(0, maleNames.size()-1);
        givenname = maleNames[i];
    } else {
        i = ri(0, femaleNames.size()-1);
        givenname = femaleNames[i];
    }

    j = ri(1, 100);
    
    if(j <= commonPercentage) {
        i = ri(0, commonFamilyNames.size()-1);
        lastname = commonFamilyNames[i];
    } else if(j > commonPercentage) {
        i = ri(0, uncommonFamilyNames.size()-1);
        lastname = uncommonFamilyNames[i];
    }

    fullname = givenname + " " + lastname;
    return fullname;
}

// vim: fdm=syntax
