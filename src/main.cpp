
#include "namegenerator.h"
#include "entity.h"
#include "person.h"
#include "utils.h"
#include "date.h"

#include <iostream>
#include <vector>
#include <memory>

using namespace std;

// Boost random number library
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

int seed;
boost::random::mt19937 rng;
NameGenerator *n;

vector<std::shared_ptr<Person>> pop;


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
	    pop.push_back(p);
    }
}

void lookForNewPeople(Date d)
{
    if(one_in(100)) {
        int num = ri(1, 2);
        for(int i = 0; i < num; ++i) {
            std::shared_ptr<Person> p;
            p = make_shared<Person>();

            p->generateRandom();
            p->setBornHere(false);
            p->setMovedYear(d.getYear());
            pop.push_back(p);
        }
    }
}

void lookForPartners(Date d)
{
    int year = d.getYear();

    for(auto m : pop) {
        for(auto f : pop) {
            if(!m->isMarried() && year >= m->getBirthYear() + 18) {
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
            
            if(!f->isMarried() && year >= f->getBirthYear() + 18) {
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


void processDay(Date d)
{
    lookForNewPeople(d);
    lookForPartners(d);
}

void simulate()
{
    Date startDate = Date(1670, 1, 1);
    int years = 10;

    for(int i = 0; i < (365 * years); i++) {
        ++startDate;
        processDay(startDate);
    }

}

int main()
{
	seed = time(0);
	rng.seed(seed);

	n = new NameGenerator();

    cout << endl << endl << " P O P U L A T I O N   G E N E R A T O R " << endl << endl;

    setupInitialPopulation();
    simulate();

    for(auto it : pop) {
        it->describe();
    }


	return 0;
}
