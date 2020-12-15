#include <iostream>
#include <sstream>
#include <string>

#include "config.h"
#include "date.h"
#include "name.h"
#include "person.h"
#include "population.h"
#include "utils.h"

// Boost random number library
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

extern int seed;
extern const int yearzero = 1660;
extern boost::random::mt19937 rng;
extern NameGenerator *n;
extern Population population;
extern struct Statistics globalStatistics;
extern ConfigData c;

// Methods for Person class

void Person::generateRandom()
{
    if (fiftyfifty()) {
        setName(n->generateName(true));
        setGender(male);
    } else {
        setName(n->generateName(false));
        setGender(female);
    }

    int birthyear = yearzero + ri(-50, 5);
    int birthmonth = ri(1, 12);
    int birthday = ri(1, 28);
    Date d = Date(birthyear, birthmonth, birthday);

    BirthEvent *b = new BirthEvent(shared_from_this(), d);
    ev.push_back(b);
}

void Person::generateRandom(Date bd)
{
    if (fiftyfifty()) {
        setName(n->generateName(true));
        setGender(male);
    } else {
        setName(n->generateName(false));
        setGender(female);
    }

    BirthEvent *b = new BirthEvent(shared_from_this(), bd);
    ev.push_back(b);
}

std::string Person::getName()
{
    if (!isMarried())
        return name.get();
    else
        return name.getFullMarried();
}

int Person::getBirthYear()
{
    for (auto it : ev) {
        if (it->getType() == etBirth)
            return it->getDate().getYear();
    }
    return 0;
};

Date Person::getBirthday()
{
    for (auto it : ev) {
        if (it->getType() == etBirth)
            return it->getDate();
    }
    return Date(0, 0, 0);
};

int Person::getMarriageYear()
{
    for (auto it : ev) {
        if (it->getType() == etMarriage)
            return it->getDate().getYear();
    }
    return 0;
};

Date Person::getMarriageDate()
{
    for (auto it : ev) {
        if (it->getType() == etMarriage)
            return it->getDate();
    }
    return Date(0, 0, 0);
};

Date Person::getDeathDate()
{
    for (auto it : ev) {
        if (it->getType() == etDeath)
            return it->getDate();
    }
    return Date(0, 0, 0);
}

Date Person::getWidowDate()
{
    for (auto it : ev) {
        if (it->getType() == etWidow)
            return it->getDate();
    }
    return Date(0, 0, 0);
}

// Get a person's age at a given date, checking whether or not it is on/after the person's birthday that year.
int Person::getAge(Date d)
{
    // TODO: compare dates with operator overloading in Date class? find out.
    int age;
    Date bd = getBirthday();
    age = d.getYear() - bd.getYear();

    if (d.getMonth() < bd.getMonth()) {
        age--;
        return age;
    }

    if (d.getMonth() == bd.getMonth()) {
        if (d.getDay() < bd.getDay()) {
            age--;
            return age;
        }
    }

    return age;
}

// Sets a person's parents, and adds person to parents' children
void Person::setParents(std::shared_ptr<Person> m, std::shared_ptr<Person> f)
{
    this->mother = m;
    this->mother->children.push_back(shared_from_this());
    this->father = f;
    this->father->children.push_back(shared_from_this());
};

// fuck, marry, kill ... heh

void Person::fuck(std::shared_ptr<Person> partner, Date d)
{
    // TODO: small chance of pregnancy also after maxAge
    if (getAge(d) <= c.maxAgeForPregnancy && spouse->getAge(d) <= c.maxAgeForPregnancy && one_in(300)) {
        if (gender == male && partner->getGender() == female && !partner->isPregnant())
            partner->impregnate(d);
        if (gender == female && partner->getGender() == male && !isPregnant())
            impregnate(d);
    }

    partner->statistics.sexytimes++;
    statistics.sexytimes++;
}

void Person::marry(std::shared_ptr<Person> spouse, Date date)
{
    if (married) {
        dbg("ERROR: %s is already married!", name.get().c_str());
    }

    setSpouse(spouse);
    setMarried(true);
    spouses.push_back(spouse);

    if (gender == female)
        name.setMarried(spouse->getFamilyName());
    else
        name.setMarried(getFamilyName());

    MarriageEvent *m = new MarriageEvent(shared_from_this(), date, spouse);
    ev.push_back(m);

    statistics.marriages++;
    globalStatistics.marriages++;
}

// TODO: cancel scheduled events (like childbirth) when a person dies.
void Person::kill(Date d)
{
    setAlive(false);
    DeathEvent *event;

    event = new DeathEvent(shared_from_this(), d);
    ev.push_back(event);

    if (married && spouse->isAlive())
        spouse->makeWidow(d);

    globalStatistics.deaths++;
}

void Person::kill(Date d, std::string reason)
{
    setAlive(false);
    DeathEvent *event;

    event = new DeathEvent(shared_from_this(), d, reason);
    ev.push_back(event);

    if (married && spouse->isAlive())
        spouse->makeWidow(d);

    globalStatistics.deaths++;
}

void Person::impregnate(Date d)
{
    PregnantEvent *preggers = new PregnantEvent(shared_from_this(), d, spouse);
    setPregnant(true);
    ev.push_back(preggers);
    statistics.pregnancies++;
    globalStatistics.pregnancies++;

    Date cbd = d + (268 + ri(-10, 10)); // 268 is the median length of a human pregnancy, and 70% give birth within +/- 10 days of this date, statistically. Or so I've read.
    ChildbirthEvent *cb = new ChildbirthEvent(shared_from_this(), cbd);
    sched.push_back(cb);
}

std::shared_ptr<Person> Person::giveBirth(Date d)
{
    // TODO: set parents etc upon conception!
    // TODO: add stillbirth :(
    std::shared_ptr<Person> child;
    child = population.spawnPerson();
    if (spouse) {
        child->setParents(shared_from_this(), getSpouse()->shared_from_this());
    } else {
        // for now, assume the last spouse is the father...
        std::shared_ptr<Person> father = spouses.back();
        child->setParents(shared_from_this(), father);
    }
    child->generateRandom(d);
    child->setBornHere(true);
    child->name.setFamily(child->father->getFamilyName());
    globalStatistics.births++;
    globalStatistics.totalNumberOfPeople++;

    if (one_in(c.childBirthDeathFrequency)) {
        kill(d, "died during child birth");
        globalStatistics.deathsChildBirth++;
        if (fiftyfifty()) {
            child->kill(d, "died from complications during " + child->getPossessivePronoun() + " birth");
            globalStatistics.deathsOwnBirth++;
        }
    }

    return child;
}

// Make a person a widow(er)
// outdated, but not handled correctly: For now, widow(er)s don't remarry, so married flag stays true.
void Person::makeWidow(Date d)
{
    WidowEvent *wid = new WidowEvent(shared_from_this(), d, spouse);
    ev.push_back(wid);
    setMarried(false);
    spouse = nullptr;
}

void Person::makeOrphan(Date d)
{
    OrphanEvent *oe = new OrphanEvent(shared_from_this(), d);
    ev.push_back(oe);
    orphan = true;
}

void Person::checkOldAge(Date d)
{
    int age = this->getAge(d);
    double a = (double)age;

    if (age >= 60) {
        double riskOfDying = (pow(2.0, (double)(a / 15.0))) / 4.0;
        int risk = (int)riskOfDying;
        //dbg("age = %d   riskOfDying = %f    risk = %d", age, riskOfDying, risk);
        if (x_in_y(risk, 100)) {
            kill(d, "died from old age");
            globalStatistics.deathsOldAge++;
        }
    }
}

// TODO: this should be more flexible, data-driven
// TODO: take age more into account!
void Person::deathForVariousReasons(Date d)
{
    int i = ri(1, 12);
    if (i == 1) {
        kill(d, "died of a mysterious illness");
        globalStatistics.deathsIllness++;
    } else if (i == 2 && getAge(d) > 13 && fiftyfifty()) {
        kill(d, "committed suicide");
        globalStatistics.deathsSuicide++;
    } else if (i == 3) {
        kill(d, "was attacked and killed by an animal");
        globalStatistics.deathsAttack++;
    } else if (i == 4) {
        kill(d, "was killed in an accident");
        globalStatistics.deathsAccident++;
    } else if (i == 5) {
        if (getAge(d) <= 4 || one_in(100)) {
            kill(d, "drowned accidentally");
        } else if (getAge(d) <= 10 || one_in(100)) {
            kill(d, "drowned while swimming");
        } else {
            kill(d, "drowned at sea");
        }
        globalStatistics.deathsDrowned++;
    } else if (i == 7) {
        int x = ri(1, 11);
        switch (x) {
        case 1:
            kill(d, "died of whooping cough");
            break;
        case 2:
            kill(d, "died of diphteria");
            break;
        case 3:
            kill(d, "died of dysentery");
            break;
        case 4:
            kill(d, "died of tuberculosis");
            break;
        case 5:
            kill(d, "died of typhus");
            break;
        case 6:
            kill(d, "died of typhoid fever");
            break;
        case 7:
            kill(d, "died of rickets");
            break;
        case 8:
            kill(d, "died of chicken pox");
            break;
        case 9:
            kill(d, "died of measles");
            break;
        case 10:
            kill(d, "died of scarlet fever");
            break;
        case 11:
            kill(d, "died of smallpox");
            break;
        }
        globalStatistics.deathsIllness++;
    } else {
        kill(d);
        globalStatistics.deathsUnknown++;
    }
}

// let's not have babies accidentally drown. But a 4yo child or older (or even a little younger) could potentially wander off and fall into the ocean and drown. Life is rough.
void Person::describe(Date d, bool stats)
{
    std::stringstream description;

    for (auto it : ev) { // TODO: find? search? separate methods for getting birth event etc?
        if (it->getType() == etBirth) {
            BirthEvent *b = dynamic_cast<BirthEvent *>(it);
            description << b->describe();
        }
    }

    if (!mother && !father) {
        description << "It is not known who " << getPossessivePronoun() << " parents were." << std::endl;
    } else {
        if (mother && father) {
            description << cap(getPossessivePronoun()) << " parents were " << mother->getGivenName() << " and " << father->getName() << "." << std::endl;
        } /*else if(mother && !father)
            {if(mother)
            description << getName() << "'s mother was " << mother->getName() << std::endl;
        if(father)
            description << getName() << "'s father was " << father->getName() << std::endl;*/
    }

    if (bornHere) {
        description << cap(getPersonalPronoun()) << " was born here in Collinsport." << std::endl;
    } else {
        for (auto it : ev) {
            if (it->getType() == etMigration) {
                MigrationEvent *m = dynamic_cast<MigrationEvent *>(it);
                description << m->describe();
            }
        }
    }

    // if (isMarried()) {
    // if (statistics.marriages > 0) {
    for (auto it : ev) {
        auto eventType = it->getType();
        if (eventType == etMarriage) {
            MarriageEvent *e = dynamic_cast<MarriageEvent *>(it);
            description << e->describe();
        } else if (eventType == etPregnant) {
            PregnantEvent *e = dynamic_cast<PregnantEvent *>(it);
            description << e->describe();
        } else if (eventType == etChildbirth) {
            ChildbirthEvent *e = dynamic_cast<ChildbirthEvent *>(it);
            description << e->describe();
        } else if (eventType == etWidow) {
            WidowEvent *e = dynamic_cast<WidowEvent *>(it);
            description << e->describe();
        } else if (eventType == etOrphan) {
            OrphanEvent *e = dynamic_cast<OrphanEvent *>(it);
            description << e->describe();
        } else {
            // description << " ERROR - unknown event found!" << std::endl;
        }
    }
    // } else {
    if (statistics.marriages == 0 && getAge(getDeathDate()) >= c.ageAdult)
        description << cap(getPersonalPronoun()) << " never married." << std::endl;
    // }

    if (isAlive()) {
        description << "As of " << d.pp() << " " << getPersonalPronoun() << " is " << getAge(d) << " years old and still alive." << std::endl;
    } else {
        for (auto it : ev) {
            if (it->getType() == etDeath) {
                DeathEvent *dev = dynamic_cast<DeathEvent *>(it);
                description << dev->describe();
            }
        }
    }

    if (stats) {
        description << cap(getPersonalPronoun()) << " got married " << statistics.marriages << " times, and had sex " << statistics.sexytimes << " times." << std::endl;
        if (gender == female) {
            description << cap(getPersonalPronoun()) << " got pregnant " << statistics.pregnancies << " times." << std::endl;
        }
    }

    description << std::endl;
    std::cout << description.str();
}

// Various related functions

bool agesWithinReason(int a, int b)
{
    if ((max(a, b) - min(a, b)) <= (12 + ri(-2, 2))) // add some randomization/variability
        return true;
    else
        return false;
}

void lookForPartners(shared_ptr<Person> p, Date d)
{
    for (auto partner : population.getAllUnmarried(d)) {
        if (partner->getGender() != p->getGender() && partner->getAge(d) >= c.ageAdult && partner->getFamilyName() != p->getFamilyName()) { // no same sex marriages yet :/ maybe in the progressive future. On the bright side, also no children getting married or people with the same family name (the latter are assumed to be related).
            if ((agesWithinReason(partner->getAge(d), p->getAge(d)))) {
                p->marry(partner, d);
                partner->marry(p, d);
                return;
            }
        }
    }
}

void lookForSexyTime(shared_ptr<Person> p, Date d)
{
    if (p->isMarried() && p->getSpouse()->isAlive() && fiftyfifty()) { // could be simulated in more detail or more statistically accurate, but, well, lol, it's probably good enough! At least there's no necrophilia.
        p->fuck(p->getSpouse(), d);
        globalStatistics.sexyTimes++;
        //cout << d.pp() << ": " << p->getName() << " got Sexy Time with " << p->getSpouse()->getName() << "!" << endl;
    }
}

// vim: fdm=syntax ft=cpp
