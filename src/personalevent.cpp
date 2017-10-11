
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
    s << "In " << owner->getMarriageYear() << ", at age " << owner->getAge(owner->getMarriageDate()) << ", " << owner->getPersonalPronoun() << " got married to " << owner->getSpouse()->getName() << ", who was " << owner->getSpouse()->getAge(owner->getSpouse()->getMarriageDate()) << " years old at the time." << std::endl;
    std::string ret = s.str();
    return ret;
}

void MarriageEvent::execute()
{
}

std::string DeathEvent::describe()
{
    std::stringstream s;
    s << "On " << owner->getDeathDate().pp() << ", at age " << owner->getAge(owner->getDeathDate()) << ", " << owner->getName() << " " << reason << "." << std::endl;
    std::string ret = s.str();
    return ret;
}

void DeathEvent::execute()
{
}

