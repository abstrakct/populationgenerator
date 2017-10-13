
#include <string>
#include <sstream>
#include "personalevent.h"

std::string BirthEvent::describe()
{
    std::stringstream s;

    if(owner->getGender() == male || (owner->getGender() == female && !owner->isMarried()))
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
    if(owner->getGender() == female)
        s << "On " << date.pp() << ", at age " << owner->getAge(date) << ", " << owner->getPersonalPronoun() << " got pregnant." << endl;
    if(owner->getGender() == male)
        s << "On " << date.pp() << ", at age " << owner->getAge(date) << ", " << owner->getPossessivePronoun() << " wife got pregnant." << endl;
    std::string ret = s.str();
    return ret;
}

void PregnantEvent::execute()
{
}

std::string ChildbirthEvent::describe()
{
    std::stringstream s;
    if(owner->getGender() == female && owner->isAlive(date))
        s << "On " << date.pp() << ", at age " << owner->getAge(date) << ", " << owner->getPersonalPronoun() << " gave birth to a child." << endl;
    if(owner->getGender() == male && owner->getSpouse()->isAlive(date))
        s << "On " << date.pp() << ", at age " << owner->getAge(date) << ", " << owner->getPossessivePronoun() << " wife gave birth to a child." << endl;
    std::string ret = s.str();
    return ret;
}

void ChildbirthEvent::execute()
{
    owner->ev.emplace_back(new ChildbirthEvent(this->owner, this->getDate(), this->otherParent));
    owner->getSpouse()->ev.emplace_back(new ChildbirthEvent(this->owner->getSpouse(), this->getDate(), this->owner));
    owner->setPregnant(false);
}
