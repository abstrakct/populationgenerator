
#include <json/json.h>
#include <iostream>
#include <fstream>

#include "config.h"

extern ConfigData c;

void readConfigFiles()
{
    std::ifstream ifs(CONFIG_FILE);
    Json::CharReaderBuilder builder;
    // Json::CharReader *reader = builder.newCharReader();
    std::unique_ptr<Json::CharReader> const reader(builder.newCharReader());
    Json::Value root;
    std::string errors;

    std::cout << "Reading config file: " << CONFIG_FILE << std::endl;

    ifs >> root;

    // std::cout << root << std::endl;

    char const *begin, *end;
    root.getString(&begin, &end);
    reader->parse(begin, end, &root, &errors);

    Json::Value &options = root["options"];

    // std::cout << options["simulation"][0]["yearZero"] << std::endl;

    // TODO: add error checking / try/catch exceptions

    c.yearZero = options["simulation"][0]["yearZero"].asInt();
    c.simulationYears = options["simulation"][0]["simulationYears"].asInt();
    c.initialPopulationMin = options["simulation"][0]["initialPopulationMin"].asInt();
    c.initialPopulationMax = options["simulation"][0]["initialPopulationMax"].asInt();
    c.ageAdult = options["simulation"][0]["ageAdult"].asInt();
    c.marriageFrequency = options["simulation"][0]["marriageFrequency"].asInt();
    c.sexyTimeFrequency = options["simulation"][0]["sexyTimeFrequency"].asInt();
    c.unexpectedDeathFrequency = options["simulation"][0]["unexpectedDeathFrequency"].asInt();
    c.oldAgeDeathFrequency = options["simulation"][0]["oldAgeDeathFrequency"].asInt();
    c.childBirthDeathFrequency = options["simulation"][0]["childBirthDeathFrequency"].asInt();
    c.immigrationFrequency = options["simulation"][0]["immigrationFrequency"].asInt();
}
