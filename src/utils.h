#pragma once

#include <string>
#include <cctype>
#include <boost/serialization/base_object.hpp>
#include "debug.h"

#define CALL_MEMBER_FN(object, ptr) ((object).*(ptr))()

struct Statistics {
    int initialPopulation;
    int totalNumberOfPeople;
    int immigrants;
    int marriages;
    int sexyTimes, pregnancies, births;
    int deaths, deathsOldAge, deathsUnknown;
};

signed int ability_modifier(int ab);
int ri(int a, int b);
bool fiftyfifty();
bool x_in_y(int x, int y);
bool one_in(int chance);
int dice(int num, int sides, signed int modifier);
int roll_die();
std::string cap(std::string s);
void printStatistics(struct Statistics stat);


// vim: fdm=syntax

// vim: fdm=syntax ft=cpp
