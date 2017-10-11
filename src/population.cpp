
#include "population.h"


void Population::addPerson(std::shared_ptr<Person> p)
{
    pop.push_back(p);
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
