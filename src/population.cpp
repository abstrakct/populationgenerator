
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

    for(auto it : pop) {        // there are probably more elegant solutions to this, probably involving lambdas and search functions in the APjkkI
        if(!it->isMarried() && it->isAlive()) {
            ret.push_back(it);
        }
    }

    return ret;
}


std::vector<std::shared_ptr<Person>> Population::getAllDead()
{
    std::vector<std::shared_ptr<Person>> ret;

    for(auto it : pop) {
        if(!it->isAlive())
            ret.push_back(it);
    }

    return ret;
}


// vim: fdm=syntax ft=cpp
