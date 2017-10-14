

#include <string>
#include <sstream>
#include "date.h"

std::string monthnames[13] = { "", "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
int monthdays[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

std::string Date::pp()
{
    std::stringstream s;

    s << monthnames[month] << " " << day << ", " << year;

    std::string ret = s.str();
    return ret;
}

void Date::incYear(int a)
{
    year += a;
}

void Date::incMonth(int a)
{
    month += a;
    if(month > 12) {
        incYear();
        month -= 12;    // TODO: checks for more than 12 months
    }
}

void Date::incDay(int a)
{
    day += a;                          // TODO: checks for incrementing more than 28-31 days
    if(day > monthdays[month]) {
        incMonth();
        day = 1;
    }
}



// vim: fdm=syntax ft=cpp
