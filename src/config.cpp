
#include <json/json.h>
#include <iostream>
#include <fstream>

#include "config.h"

extern ConfigData c;

void readConfigFiles()
{
    std::ifstream ifs(CONFIG_FILE);
    Json::CharReaderBuilder builder;
    Json::CharReader *reader = builder.newCharReader();
    Json::Value root;
    std::string errors;

    ifs >> root;
    reader->parse(root.asString().c_str(), root.asString().c_str() + root.asString().size(), &root, &errors);
    Json::Value& simulationoptions = root["simulationoptions"];

    // TODO: add error checking / try/catch exceptions 

    c.yearZero = simulationoptions[0]["yearZero"].asInt();
    c.simulationYears = simulationoptions[0]["simulationYears"].asInt();
    c.initialPopulationMin = simulationoptions[0]["initialPopulationMin"].asInt();
    c.initialPopulationMax = simulationoptions[0]["initialPopulationMax"].asInt();
    c.ageAdult = simulationoptions[0]["ageAdult"].asInt();
    c.marriageFrequency = simulationoptions[0]["marriageFrequency"].asInt();
    c.sexyTimeFrequency = simulationoptions[0]["sexyTimeFrequency"].asInt();
    c.unexpectedDeathFrequency = simulationoptions[0]["unexpectedDeathFrequency"].asInt();
    c.oldAgeDeathFrequency = simulationoptions[0]["oldAgeDeathFrequency"].asInt();
    c.childBirthDeathFrequency = simulationoptions[0]["childBirthDeathFrequency"].asInt();
    c.immigrationFrequency = simulationoptions[0]["immigrationFrequency"].asInt();
}

