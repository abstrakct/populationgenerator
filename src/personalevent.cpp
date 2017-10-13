
#include <string>
#include <sstream>
#include "personalevent.h"

std::string BirthEvent::describe()
{
    std::stringstream s;

    if(owner->getGender() == male)
        s << owner->getName() << " was born " << date.pp() << "." << std::endl;
    if(owner->getGender() == female && owner->isMarried())
        s << owner->getName() << ", née " << owner->getFamilyName() << ", was born " << date.pp() << "." << std::endl;

    std::string ret = s.str();
    return ret;
}

void BirthEvent::execute()
{
}

std::string MarriageEvent::describe()
{
    std::stringstream s;
    s << "On " << date.pp() << ", at age " << owner->getAge(date) << ", " << owner->getPersonalPronoun() << " got married to " << owner->getSpouse()->getName() << ", who was " << owner->getSpouse()->getAge(owner->getSpouse()->getMarriageDate()) << " years old at the time." << std::endl;
    std::string ret = s.str();
    return ret;
}

void MarriageEvent::execute()
{
}

std::string DeathEvent::describe()
{
    std::stringstream s;
    s << "On " << date.pp() << ", at age " << owner->getAge(owner->getDeathDate()) << ", " << owner->getPersonalPronoun() << " " << reason << "." << std::endl;
    std::string ret = s.str();
    return ret;
}

void DeathEvent::execute()
{
}

std::string WidowEvent::describe()
{
    std::stringstream s;
    s << "On " << owner->getSpouse()->getDeathDate().pp() << ", at age " << owner->getAge(date) << ", " << owner->getPersonalPronoun() << " became a " << (owner->getGender() == female ? "widow" : "widower") << "." << std::endl;
    std::string ret = s.str();
    return ret;
}

void WidowEvent::execute()
{
}

std::string PregnantEvent::describe()
{
    std::stringstream s;
    //if(owner->getGender() == female)
        s << "On " << date.pp() << ", at age " << owner->getAge(date) << ", " << owner->getPersonalPronoun() << " became pregnant!" << endl;
    //if(owner->getGender() == male)
    //    s << "On " << date.pp() << ", at age " << owner->getAge(date) << ", " << owner->getPossessivePronoun() << " wife became pregnant!" << endl;
    std::string ret = s.str();
    return ret;
}

void PregnantEvent::execute()
{
}

