
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
    s << "In " << owner->getMarriageYear() << ", at age " << owner->getAge(owner->getMarriageYear()) << ", " << owner->getPersonalPronoun() << " got married to " << owner->getSpouse()->getName() << ", who was " << owner->getSpouse()->getAge(owner->getSpouse()->getMarriageYear()) << " years old at the time." << std::endl;
    std::string ret = s.str();
    return ret;
}

void MarriageEvent::execute()
{
}

