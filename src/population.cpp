
#include "population.h"

void Population::addPerson(std::shared_ptr<Person> p)
{
    pop.push_back(p);
}

std::shared_ptr<Person> Population::spawnPerson()
{
    std::shared_ptr<Person> person = make_shared<Person>();
    person->setAlive();
    pop.push_back(person);
    return person;
}

std::vector<std::shared_ptr<Person>> Population::getAll()
{
    return pop;
}

std::vector<std::shared_ptr<Person>> Population::getAllUnmarried()
{
    std::vector<std::shared_ptr<Person>> ret;

    for (auto it : pop) { // there are probably more elegant solutions to this, probably involving lambdas and search functions in the APjkkI
        if (!it->isMarried() && it->isAlive()) {
            ret.push_back(it);
        }
    }

    return ret;
}

// Get all - but make sure they are alive at Date d
std::vector<std::shared_ptr<Person>> Population::getAllUnmarried(Date d)
{
    std::vector<std::shared_ptr<Person>> ret;

    for (auto it : pop) {
        if (!it->isMarried() && it->isAlive(d)) {
            ret.push_back(it);
        }
    }

    return ret;
}

std::vector<std::shared_ptr<Person>> Population::getAllDead()
{
    std::vector<std::shared_ptr<Person>> ret;

    for (auto it : pop) {
        if (!it->isAlive())
            ret.push_back(it);
    }

    return ret;
}

std::vector<std::shared_ptr<Person>> Population::getAllAlive()
{
    std::vector<std::shared_ptr<Person>> ret;

    for (auto it : pop) {
        if (it->isAlive())
            ret.push_back(it);
    }

    return ret;
}

std::vector<std::shared_ptr<Person>> Population::getAllAliveCouples()
{
    std::vector<std::shared_ptr<Person>> ret;

    for (auto it : pop) {
        if (it->isAlive() && it->getGender() == male && it->isMarried() && it->getSpouse()->isAlive())
            ret.push_back(it);
    }

    return ret;
}

std::list<shared_ptr<Person>> Population::getAllSortedList()
{
    std::list<shared_ptr<Person>> ret;
    for (auto it : pop) {
        ret.push_back(it);
    }

    ret.sort();
    return ret;
}

// vim: fdm=syntax ft=cpp
