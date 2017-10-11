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

    //description << getName() << " was born in " << getBirthYear() << ". ";
    
    for(auto it : ev) {     // TODO: find? search? separate methods for getting birth event etc?
        if(it->getType() == etBirth) {
            BirthEvent *b = dynamic_cast<BirthEvent*>(it);
            description << b->describe();
        }
    }

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
        for(auto it : ev) {
            if(it->getType() == etMarriage) {
                MarriageEvent *m = dynamic_cast<MarriageEvent*>(it);
                description << m->describe();
            }
        }
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

    int birthyear = 1660 + ri(-10, 10);
    int birthmonth = ri(1, 12);
    int birthday = ri(1,28);
    Date d = Date(birthyear, birthmonth, birthday);

    BirthEvent *b = new BirthEvent(shared_from_this(), d);
    ev.push_back(b);
}


int Person::getBirthYear() { 
    for(auto it : ev) {
        if(it->getType() == etBirth)
            return it->getDate().getYear();
    }
    return 0;
};

int Person::getMarriageYear() { 
    for(auto it : ev) {
        if(it->getType() == etMarriage)
            return it->getDate().getYear();
    }
    return 0;
};

void Person::marry(std::shared_ptr<Person> spouse, int date)
{
    setSpouse(spouse);
    spouse->setSpouse(shared_from_this());
    setMarried(true);
    spouse->setMarried(true);

    int y = date;
    int mo = ri(1,12);
    int d = ri(1,28);
    Date x = Date(y, mo, d);

    MarriageEvent *m = new MarriageEvent(shared_from_this(), x);
    ev.push_back(m);
}
