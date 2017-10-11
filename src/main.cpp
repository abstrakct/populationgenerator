
#include "namegenerator.h"
#include "entity.h"
#include "person.h"
#include "utils.h"
#include "date.h"
#include "population.h"

#include <iostream>
#include <vector>
#include <memory>

using namespace std;

// Boost random number library
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

int seed;
const int yearzero = 1660;
boost::random::mt19937 rng;
NameGenerator *n;

// vector<std::shared_ptr<Person>> pop;
Population pop;


void setupInitialPopulation()
{
    int num;

	num = ri(5, 10);
	num = ri(num, 20);

	for(int i = 0; i < num; ++i) {
        std::shared_ptr<Person> p;
	    p = make_shared<Person>();

	    p->generateRandom();
	    p->setBornHere(true);
        pop.addPerson(p);
    }
}

void lookForNewPeople(Date d)
{
    int num = ri(1, 2);
    for(int i = 0; i < num; ++i) {
        std::shared_ptr<Person> p;
        p = make_shared<Person>();

        p->generateRandom();
        p->setBornHere(false);
        p->setMovedYear(d.getYear());
        pop.addPerson(p);
    }
}

/*
void lookForPartners(Date d)
{
    int year = d.getYear();

    for(auto m : pop.getAllUnmarried()) {             // no adultery supported for now!
        for(auto f : pop.getAllUnmarried()) {
            if(year >= m->getBirthYear() + 18) {
                if(f != m) {
                    if(m->getGender() == male && f->getGender() == female) {
                        if(year >= f->getBirthYear() + 18) {
                            if(!f->isMarried()) {
                                if(one_in(70) && f->getAge(year) <= (m->getAge(year) + ri(-3,3))) {
                                    m->marry(f, year);
                                    f->marry(m, year);
                                }
                            }
                        }
                    }
                }
            }
            
            if(year >= f->getBirthYear() + 18) {
                if(m != f) {
                    if(f->getGender() == female && m->getGender() == male) {
                        if(year >= m->getBirthYear() + 18) {
                            if(!m->isMarried()) {
                                if(one_in(70) && f->getAge(year) <= (m->getAge(year) + ri(-3,3))) {
                                    f->marry(m, year);
                                    m->marry(f, year);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
*/

void processDay(Date d)
{
    //int year = d.getYear();

    // Step 1:
    // Go through all the people, see if something should be done to anyone...
    for(auto it : pop.getAll()) {
        if(it->isAlive()) {
            if(one_in(250))
                it->checkUnexpectedDeath(d);
            if(!it->isMarried() && it->getAge(d) >= 18 && one_in(30))
                lookForPartners(it, d);
        }
    }

    // Step 2:
    // Check for external events
    //if(one_in(250))
    //    lookForNewPeople(d);
}

void simulate()
{
    Date startDate = Date(yearzero + 10, 1, 1);
    int years = 50;

    for(int i = 0; i < (365 * years); i++) {
        ++startDate;
        processDay(startDate);
    }
}

void simulateasdf()
{
}

int main()
{
	seed = time(0);
	rng.seed(seed);

	n = new NameGenerator();

    cout << endl << endl << " P O P U L A T I O N   G E N E R A T O R " << endl << endl;

    setupInitialPopulation();
    simulate();

    // idea: output to textfile! name yyyymmddhhmmss or something.
    for(auto it : pop.getAll()) {
        it->describe();
    }

	return 0;
}
