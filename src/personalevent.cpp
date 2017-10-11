
#include <string>
#include <sstream>
#include "personalevent.h"

std::string BirthEvent::describe()
{
    std::stringstream s;

    s << owner->getName() << " was born " << getDate().pp() << "." << std::endl;

    std::string ret = s.str();
    return ret;
}

void BirthEvent::execute()
{
}

std::string MarriageEvent::describe()
{
    std::stringstream s;
    s << "On " << owner->getMarriageDate().pp() << ", at age " << owner->getAge(owner->getMarriageDate()) << ", " << owner->getPersonalPronoun() << " got married to " << owner->getSpouse()->getName() << ", who was " << owner->getSpouse()->getAge(owner->getSpouse()->getMarriageDate()) << " years old at the time." << std::endl;
    std::string ret = s.str();
    return ret;
}

void MarriageEvent::execute()
{
}

std::string DeathEvent::describe()
{
    std::stringstream s;
    s << "On " << owner->getDeathDate().pp() << ", at age " << owner->getAge(owner->getDeathDate()) << ", " << owner->getPersonalPronoun() << " " << reason << "." << std::endl;
    std::string ret = s.str();
    return ret;
}

void DeathEvent::execute()
{
}

std::string WidowEvent::describe()
{
    std::stringstream s;
    s << "On " << owner->getSpouse()->getDeathDate().pp() << ", at age " << owner->getAge(owner->getWidowDate()) << ", " << owner->getPersonalPronoun() << " became a " << (owner->getGender() == female ? "widow" : "widower") << "." << std::endl;
    std::string ret = s.str();
    return ret;
}

void WidowEvent::execute()
{
}

