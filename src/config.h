
/*
 * Reading configuration / data from JSON files!
 */

#pragma once

#define CONFIG_FILE "data/config.json"

void readConfigFiles();

struct ConfigData {
    int yearZero;
    int simulationYears;
    int initialPopulationMin, initialPopulationMax;
    int ageAdult;
    int marriageFrequency, sexyTimeFrequency, unexpectedDeathFrequency, oldAgeDeathFrequency, childBirthDeathFrequency;
    int immigrationFrequency;
    int maxAgeForPregnancy;
    int orphanMaxAge;
};
