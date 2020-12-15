
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "debug.h"
#include "p-event.h"
#include "population.h"

extern Population pop;

std::string BirthEvent::describe()
{
    std::stringstream s;

    if (owner->getGender() == male || (owner->getGender() == female && !owner->isMarried()))
        s << owner->getName() << " was born " << date.pp() << "." << std::endl;
    if (owner->getGender() == female && owner->isMarried())
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
    s << "On " << date.pp() << ", at age " << owner->getAge(date) << ", " << owner->getPersonalPronoun() << " got married to " << spouse->getMaidenName() << ", who was " << spouse->getAge(date) << " years old at the time." << std::endl;
    // s << "IF: " << std::fixed << std::setprecision(1) << owner->getGenetics().infertility_factor << "  Spouse IF: " << std::fixed << std::setprecision(1) << spouse->getGenetics().infertility_factor << std::endl;
    std::string ret = s.str();
    return ret;
}

void MarriageEvent::execute()
{
}

std::string DeathEvent::describe()
{
    std::stringstream s;
    s << "On " << date.pp() << ", at age " << owner->getAge(owner->getDeathDate()) << ", " << owner->getName() << " " << reason << "." << std::endl;
    std::string ret = s.str();
    return ret;
}

void DeathEvent::execute()
{
}

std::string WidowEvent::describe()
{
    std::stringstream s;
    s << "On " << spouse->getDeathDate().pp() << ", at age " << owner->getAge(date) << ", " << owner->getPersonalPronoun() << " became a " << (owner->getGender() == female ? "widow" : "widower") << "." << std::endl;
    std::string ret = s.str();
    return ret;
}

void WidowEvent::execute()
{
}

std::string PregnantEvent::describe()
{
    std::stringstream s;
    if (owner->getGender() == female)
        s << "On " << date.pp() << ", at age " << owner->getAge(date) << ", " << owner->getPersonalPronoun() << " got pregnant." << endl;
    if (owner->getGender() == male)
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
    if (owner->getGender() == female)
        s << "On " << date.pp() << ", at age " << owner->getAge(date) << ", " << owner->getPersonalPronoun() << " gave birth to a " << (child->getGender() == male ? "boy" : "girl") << " named " << child->getMaidenName() << "." << endl;
    if (owner->getGender() == male)
        s << "On " << date.pp() << ", at age " << owner->getAge(date) << ", " << owner->getPossessivePronoun() << " wife gave birth to a " << (child->getGender() == male ? "boy" : "girl") << " named " << child->getMaidenName() << "." << endl;
    std::string ret = s.str();
    return ret;
}

void ChildbirthEvent::execute()
{
    if (!owner->isAlive(date)) {
        std::cerr << owner->getName() << " is trying to give birth, but isn't alive! That shouldn't happen!" << endl;
        owner->describe(date);
        exit(1);
    }

    std::shared_ptr<Person> father = owner->getSpouse();
    ChildbirthEvent *cbev = new ChildbirthEvent(owner, date);
    ChildbirthEvent *cbevf = new ChildbirthEvent(father, date);

    owner->setPregnant(false);
    cbev->child = owner->giveBirth(date);
    owner->ev.push_back(cbev);

    cbevf->owner = father;
    cbevf->child = cbev->child;
    father->ev.push_back(cbevf);
}

std::string MigrationEvent::describe()
{
    std::stringstream s;
    if (owner->isAlive(date))
        s << "On " << date.pp() << " " << owner->getPersonalPronoun() << " moved to Collinsport." << endl;
    std::string ret = s.str();
    return ret;
}

void MigrationEvent::execute()
{
}

std::string OrphanEvent::describe()
{
    std::stringstream s;
    if (owner->isAlive(date))
        s << "On " << date.pp() << ", at age " << owner->getAge(date) << ", " << owner->getPersonalPronoun() << " became an orphan when " << owner->getPossessivePronoun() << " last remaining parent died." << endl;
    std::string ret = s.str();
    return ret;
}

void OrphanEvent::execute()
{
}

// vim: fdm=syntax ft=cpp
