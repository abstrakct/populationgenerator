/* 
 * Various utility functions
 */

#include <cctype>
#include <iostream>
#include <stdlib.h>
#include <string>
using namespace std;

#include "utils.h"

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

extern boost::random::mt19937 rng;
extern long seed;

int ri(int a, int b)
{
    boost::random::uniform_int_distribution<> dist(a, b);
    return dist(rng);
}

bool fiftyfifty()
{
    int i;

    boost::random::uniform_int_distribution<> dist(1, 100);
    i = dist(rng);

    if (i <= 50)
        return true;
    else
        return false;
}

bool x_in_y(int x, int y)
{
    int i;

    boost::random::uniform_int_distribution<> dist(1, y);
    i = dist(rng);

    if (i <= x)
        return true;
    else
        return false;
}

bool one_in(int chance)
{
    if (chance < 1)
        return false;
    return x_in_y(1, chance);
}

int roll_die()
{
    boost::random::uniform_int_distribution<> dist(1, 6);
    return dist(rng);
}

int dice(int num, int sides, signed int modifier)
{
    int min, max;

    min = num;
    max = num * sides;
    boost::random::uniform_int_distribution<> dist(min, max);
    return (dist(rng) + modifier);
}

signed int ability_modifier(int ab)
{
    return ((ab / 2) - 5);
}

std::string cap(std::string s)
{
    std::string ret = s;
    ret[0] = toupper(ret[0]);
    return ret;
}

void printStatistics(struct Statistics stat)
{
    cout << endl
         << endl
         << " S T A T I S T I C S          seed " << seed << endl
         << endl;
    cout << "Start date of simulation:     " << stat.start.pp() << endl;
    cout << "End date of simulation:       " << stat.end.pp() << endl;
    cout << "Initial population:           " << stat.initialPopulation << endl;
    cout << "Total number of people:       " << stat.totalNumberOfPeople << endl;
    cout << "Total number of deaths:       " << stat.deaths << endl;
    cout << "Number of immigrants:         " << stat.immigrants << endl;
    cout << "Number of marriages:          " << stat.marriages << endl;
    cout << "Number of sexytimes:          " << stat.sexyTimes << endl;
    cout << "Number of pregnancies:        " << stat.pregnancies << endl;
    cout << "Number of childbirths:        " << stat.births << endl;
    cout << "Number of deaths (unknown):   " << stat.deathsUnknown << endl;
    cout << "Number of deaths (old age):   " << stat.deathsOldAge << endl;
    cout << "Number of deaths (accident):  " << stat.deathsAccident << endl;
    cout << "Number of deaths (attack):    " << stat.deathsAttack << endl;
    cout << "Number of deaths (illness):   " << stat.deathsIllness << endl;
    cout << "Number of deaths (drowned):   " << stat.deathsDrowned << endl;
    cout << "Number of deaths (suicide):   " << stat.deathsSuicide << endl;
    cout << "Number of deaths (birth):     " << stat.deathsChildBirth << endl;
    cout << "Number of deaths (own birth): " << stat.deathsOwnBirth << endl;
    cout << "Number of people alive:       " << (stat.totalNumberOfPeople - stat.deaths) << endl;
    cout << endl;
}

// vim: fdm=syntax

// vim: fdm=syntax ft=cpp
