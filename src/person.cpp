#include <iostream>
#include <string>
#include <sstream>

#include "person.h"
#include "utils.h"
#include "namegenerator.h"
#include "date.h"
#include "population.h"

// Boost random number library
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

extern int seed;
extern const int yearzero = 1660;
extern boost::random::mt19937 rng;
extern NameGenerator *n;
extern Population pop;

bool agesWithinReason(int a, int b)
{
    if((max(a, b) - min(a, b)) <= (12 + ri(-2, 2)))   // add some randomization/variability
        return true;
    else
        return false;
}

void lookForPartners(shared_ptr<Person> p, Date d)
{
    for(auto partner : pop.getAllUnmarried()) {
        if(partner->getGender() != p->getGender() && partner->getAge(d) >= 18 && partner->getFamilyName() != p->getFamilyName()) {     // no same sex marriages yet :/ maybe in the progressive future. On the bright side, also no children getting married or people with the same family name (the latter are assumed to be related).
            if((agesWithinReason(partner->getAge(d), p->getAge(d)))) {
                p->marry(partner, d);
                partner->marry(p, d);
                return;
            }
        }
    }
}

void lookForSexyTime(shared_ptr<Person> p, Date d)
{
    if(p->isMarried() && p->getSpouse()->isAlive() && fiftyfifty()) {      // could be simulated in more detail or more stastically accurate, but, well, lol, it's probably good enough! At least there's no necrophilia.
        p->fuck(p->getSpouse(), d);
        //cout << d.pp() << ": " << p->getName() << " got Sexy Time with " << p->getSpouse()->getName() << "!" << endl;
    }
}


// Methods for Person class
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

std::string Person::getName()
{
    if(!isMarried())
        return name.get();
    else
        return name.getFullMarried();
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

Date Person::getWidowDate() {
    for(auto it : ev) {
        if(it->getType() == etWidow)
            return it->getDate();
    }
    return Date(0, 0, 0);
}

// Get a person's age at a given date, checking whether or not it is on/after the person's birthday that year.
int Person::getAge(Date d) {
    // TODO: compare dates with operator overloading in Date class? find out.
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

// fuck, marry, kill ... heh

void Person::fuck(std::shared_ptr<Person> partner, Date d)
{
    if(getAge(d) <= 50 && one_in(300)) {
        if(gender == male && partner->getGender() == female && !partner->isPregnant())
            partner->impregnate(d);
        if(gender == female && partner->getGender() == male && !isPregnant())
            impregnate(d);
    }
}

void Person::marry(std::shared_ptr<Person> spouse, Date date)
{
    if(married) {
        dbg("%s is already married!", name.get().c_str());
    }

    setSpouse(spouse);
    setMarried(true);
    if(gender == female)
        name.setMarried(spouse->getFamilyName());
    else
        name.setMarried(getFamilyName());

    MarriageEvent *m = new MarriageEvent(shared_from_this(), date);
    ev.push_back(m);
}

void Person::kill(Date d)
{
    setAlive(false);
    DeathEvent *event = new DeathEvent(shared_from_this(), d);
    ev.push_back(event);

    if(married && spouse->isAlive())
        spouse->makeWidow(d);
}

void Person::impregnate(Date d)
{
    PregnantEvent *preggers = new PregnantEvent(shared_from_this(), d, spouse);
    setPregnant(true);
    ev.push_back(preggers);

    Date cbd = d + (268 + ri(-10, 10));         // 268 is the median length of a human pregnancy, and 70% give birth within +/- 10 days of this date, statistically. Or so I've read.
    ChildbirthEvent *cb = new ChildbirthEvent(shared_from_this(), cbd, spouse);
    sched.push_back(cb);
}

// Make a person a widow(er)
// For now, widow(er)s don't remarry, so married flag stays true.
void Person::makeWidow(Date d)
{
    WidowEvent *wid = new WidowEvent(shared_from_this(), d);
    ev.push_back(wid);
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
            if(it->getType() == etPregnant) {
                PregnantEvent *preg = dynamic_cast<PregnantEvent*>(it);
                description << preg->describe();
            }
            if(it->getType() == etChildbirth) {
                ChildbirthEvent *cb = dynamic_cast<ChildbirthEvent*>(it);
                description << cb->describe();
            }
            if(it->getType() == etWidow) {
                WidowEvent *w = dynamic_cast<WidowEvent*>(it);
                description << w->describe();
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

