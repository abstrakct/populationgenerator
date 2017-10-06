#include <iostream>
#include <string>
#include <sstream>

#include "person.h"
#include "utils.h"
#include "namegenerator.h"

// Boost random number library
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

extern int seed;
extern boost::random::mt19937 rng;
extern NameGenerator *n;

void Person::describe()
{
    std::stringstream description;

    description << getName() << " was born in " << getBirthYear() << ". ";

    if(mother == NULL && father == NULL) {
        description << "It is not known who " << getPossessivePronoun() << " parents were." << std::endl;
    } else {
        if(mother)
            description << getName() << "'s mother was " << mother->getName() << std::endl;
        if(father)
            description << getName() << "'s father was " << father->getName() << std::endl;
    }

    if(bornHere) {
        description << cap(getPersonalPronoun()) << " was born here in Collinsport." << std::endl;
    } else {
        description << cap(getPersonalPronoun()) << " moved to Collinsport in " << getMovedYear() << "." << std::endl;
    }

    if(isMarried()) {
        description << "In " << getMarriedYear() << ", at age " << getAge(getMarriedYear()) << ", " << getPersonalPronoun() << " got married to " << spouse->getName() << " (" << spouse->getAge(getMarriedYear()) << " years old)." << std::endl;
    } else {
        description << cap(getPersonalPronoun()) << " never married." << std::endl;
    }

    description << std::endl;
    std::cout << description.str();
}

void Person::generateRandom()
{
    if(fiftyfifty()) {
        setName(n->generateName(true));
        setGender(male);
    } else {
        setName(n->generateName(false));
        setGender(female);
    }

    setBirthYear(1660 + ri(-10, 10));
}
