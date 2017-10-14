
#include <string>
#include <sstream>
#include "personalevent.h"
#include "population.h"
#include "debug.h"

extern Population pop;

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
    s << "On " << date.pp() << ", at age " << owner->getAge(date) << ", " << owner->getPersonalPronoun() << " got married to " << owner->getSpouse()->getMaidenName() << ", who was " << owner->getSpouse()->getAge(owner->getSpouse()->getMarriageDate()) << " years old at the time." << std::endl;
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
    if(owner->getGender() == female)
        s << "On " << date.pp() << ", at age " << owner->getAge(date) << ", " << owner->getPersonalPronoun() << " gave birth to a child. The child was a " << (child->getGender() == male ? "boy" : "girl") << " named " << child->getMaidenName() << "." << endl;
    if(owner->getGender() == male)
        s << "On " << date.pp() << ", at age " << owner->getAge(date) << ", " << owner->getPossessivePronoun() << " wife gave birth to a child. The child was a " << (child->getGender() == male ? "boy" : "girl") << " named " << child->getMaidenName() << "." << endl;
    std::string ret = s.str();
    return ret;
}

void ChildbirthEvent::execute()
{
    if(!owner->isAlive(date))
        dbg("giving birth when not alive?!?!?!?!");

    ChildbirthEvent* cbev = new ChildbirthEvent(owner, date);
    ChildbirthEvent* cbevf = new ChildbirthEvent(owner->getSpouse(), date);

    owner->setPregnant(false);
    cbev->child = owner->giveBirth(date);
    owner->ev.push_back(cbev);
    
    cbevf->owner = owner->getSpouse();
    cbevf->child = cbev->child;
    owner->getSpouse()->ev.push_back(cbevf);
}

std::string MigrationEvent::describe()
{
    std::stringstream s;
    if(owner->isAlive(date))
        s << "On " << date.pp() << " " << owner->getPersonalPronoun() << " moved to Collinsport." << endl;
    std::string ret = s.str();
    return ret;
}

void MigrationEvent::execute()
{
}

