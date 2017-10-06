/* 
 * Various utility functions
 */

#include <stdlib.h>
#include <string>
#include <cctype>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

extern boost::random::mt19937 rng;

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

    if(i <= 50)
        return true;
    else
        return false;
}

bool x_in_y(int x, int y)
{
    int i;

    boost::random::uniform_int_distribution<> dist(1, y);
    i = dist(rng);

    if(i <= x)
        return true;
    else
        return false;
}

bool one_in(int chance)
{
    return x_in_y(1, chance);
}

int roll_die() {
    boost::random::uniform_int_distribution<> dist(1, 6);
    return dist(rng);
}

int dice(int num, int sides, signed int modifier) {
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
// vim: fdm=syntax
