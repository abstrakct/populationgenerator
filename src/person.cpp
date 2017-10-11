#include <iostream>
#include <string>
#include <sstream>

#include "person.h"
#include "utils.h"
#include "namegenerator.h"
#include "date.h"

// Boost random number library
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

extern int seed;
extern const int yearzero = 1660;
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

    if(!isAlive()) {
        for(auto it : ev) {
            if(it->getType() == etDeath) {
                DeathEvent *dev = dynamic_cast<DeathEvent*>(it);
                description << dev->describe();
            }
        }
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

    int birthyear = yearzero + ri(-50, 5);
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

Date Person::getBirthday() { 
    for(auto it : ev) {
        if(it->getType() == etBirth)
            return it->getDate();
    }
    return Date(0, 0, 0);
};

int Person::getMarriageYear() { 
    for(auto it : ev) {
        if(it->getType() == etMarriage)
            return it->getDate().getYear();
    }
    return 0;
};

Date Person::getMarriageDate() { 
    for(auto it : ev) {
        if(it->getType() == etMarriage)
            return it->getDate();
    }
    return Date(0, 0, 0);
};

Date Person::getDeathDate() {
    for(auto it : ev) {
        if(it->getType() == etDeath)
            return it->getDate();
    }
    return Date(0, 0, 0);
}

// Get a person's age at a given date, checking whether or not it is on/after the person's birthday that year.
int Person::getAge(Date d) {
    // TODO: do this with operator overloading in Date class? find out.
    int age;
    Date bd = getBirthday();
    age = d.getYear() - bd.getYear();

    if(d.getMonth() < bd.getMonth()) {
        age--;
        return age;
    }

    if(d.getMonth() == bd.getMonth()) {
        if(d.getDay() < bd.getDay()) {
            age--;
            return age;
        }
    }
    
    return age;
}

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

void Person::kill(Date d)
{
    setAlive(false);
    DeathEvent *event = new DeathEvent(shared_from_this(), d);
    ev.push_back(event);
}

void Person::checkOldAge(Date d)
{
    int year = d.getYear();
    int age = this->getAge(year);
    double a = (double) age;

    if(age >= 50) {
        double riskOfDying = pow(2.0, (double)(a / 15.0));
        int risk = (int) riskOfDying;
        //dbg("Risk of dying at age %d: %f (%d)", age, riskOfDying, risk);
    }
}

void Person::checkUnexpectedDeath(Date d)
{
    int year = d.getYear();
    int age = this->getAge(year);
    double a = (double) age;
    double riskOfDying = pow(2.0, (double)(a / 15.0));
    int risk = (int) riskOfDying;

    if(risk > 0) {
        if(x_in_y(risk, 100)) {
            kill(d);
        }
    }
}
