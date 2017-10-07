
#include "namegenerator.h"
#include "entity.h"
#include "person.h"
#include "utils.h"

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

void lookForNewPeople(int year)
{
    if(fiftyfifty()) {
        int num = ri(1, 2);
        for(int i = 0; i < num; ++i) {
            std::shared_ptr<Person> p;
            p = make_shared<Person>();

            p->generateRandom();
            p->setBornHere(false);
            p->setMovedYear(year);
            pop.push_back(p);
        }
    }
}

void lookForPartners(int year)
{
    for(auto m = pop.begin(); m != pop.end(); ++m) {
        for(auto f = pop.begin(); f != pop.end(); ++f) {
            if(!(*m)->isMarried() && year >= (*m)->getBirthYear() + 18) {
                if(f != m) {
                    if((*m)->getGender() == male && (*f)->getGender() == female) {
                        if(year >= (*f)->getBirthYear() + 18) {
                            if(!(*f)->isMarried()) {
                                //cout << m->getName() << " considers " << f->getName() << " as a potential partner." << endl;
                                if(one_in(3) && (*f)->getAge(year) <= ((*m)->getAge(year) + ri(-3,3))) {
                                    //cout << m->getName() << " gets married to " << f->getName() << "!" << endl;
                                    //(*m)->setMarried(true);
                                    //(*f)->setMarried(true);
                                    //(*m)->setSpouse(*f);
                                    //(*f)->setSpouse(*m);
                                    //(*m)->setMarriedYear(year);
                                    //(*f)->setMarriedYear(year);
                                    (*m)->marry(*f, year);
                                    (*f)->marry(*m, year);
                                }
                            }
                        }
                    }
                }
            }
            
            if(!(*f)->isMarried() && year >= (*f)->getBirthYear() + 18) {
                if(m != f) {
                    if((*f)->getGender() == female && (*m)->getGender() == male) {
                        if(year >= (*m)->getBirthYear() + 18) {
                            if(!(*m)->isMarried()) {
                                //cout << f->getName() << " considers " << m->getName() << " as a potential partner." << endl;
                                if(one_in(3) && (*f)->getAge(year) <= ((*m)->getAge(year) + ri(-3,3))) {
                                    //cout << f->getName() << " gets married to " << m->getName() << "!" << endl;
                                    //(*f)->setMarried(true);
                                    //(*m)->setMarried(true);
                                    //(*f)->setSpouse(*m);
                                    //(*m)->setSpouse(*f);
                                    //(*f)->setMarriedYear(year);
                                    //(*m)->setMarriedYear(year);
                                    (*f)->marry(*m, year);
                                    (*m)->marry(*f, year);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void processYear(int year)
{
    lookForNewPeople(year);
    lookForPartners(year);
}


int main()
{
	seed = time(0);
	rng.seed(seed);

	n = new NameGenerator();

    cout << endl << endl;
    cout << " P O P U L A T I O N   G E N E R A T O R " << endl << endl;

    setupInitialPopulation();

    int startYear = 1670;
    int endYear = 1690;

    for(int i = startYear; i != endYear; ++i) {
        processYear(i);
    }

    for(auto it = pop.begin(); it != pop.end(); ++it) {
        (*it)->describe();
    }


	return 0;
}
