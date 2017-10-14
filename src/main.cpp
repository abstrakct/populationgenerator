
#include "name.h"
#include "entity.h"
#include "person.h"
#include "utils.h"
#include "date.h"
#include "population.h"
#include "config.h"

#include <iostream>
#include <vector>
#include <memory>
#include <map>

using namespace std;

// Boost random number library
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

int seed;
boost::random::mt19937 rng;
NameGenerator *n;
Population pop;
ConfigData c;

struct Statistics stat;

void setupInitialPopulation()
{
    int num;

	num = ri(c.initialPopulationMin, c.initialPopulationMax);

	for(int i = 0; i < num; ++i) {
        std::shared_ptr<Person> p;

        p = pop.spawnPerson();
	    p->generateRandom();
	    p->setBornHere(true);
    }

    // TODO: add some that are already couples?
    stat.initialPopulation = num;
    stat.totalNumberOfPeople = num;
}

void lookForImmigrants(Date d)
{
        std::shared_ptr<Person> p;
        p = pop.spawnPerson();

        int birthyear = d.getYear() + ri(-38, -18);       // immigrants are between 18 - 38 years old
        int birthmonth = ri(1, 12);
        int birthday = ri(1,28);
        Date bd = Date(birthyear, birthmonth, birthday);

        p->generateRandom(bd);
        p->setBornHere(false);
        MigrationEvent *mig;
        mig = new MigrationEvent(p, d);
        p->ev.push_back(mig);
        stat.immigrants++;
        stat.totalNumberOfPeople++;
}

void processDay(Date d)
{
    bool finishedForTheDay = false;

    while(!finishedForTheDay) {

        // Step 1:
        // Go through all the people, see if something should be done to anyone...
        for(auto it : pop.getAll()) {
            if(it->isAlive()) {
                if(!it->isMarried() && it->getAge(d) >= c.ageAdult && one_in(c.marriageFrequency)) // 1 in 30
                    lookForPartners(it, d);
                if(one_in(c.sexyTimeFrequency))         // 1 in 5
                    lookForSexyTime(it, d);
                if(one_in(c.unexpectedDeathFrequency))  // 1 in 20000
                    it->deathForUnknownReasons(d);
                if(one_in(c.oldAgeDeathFrequency))      // 1 in 200
                    it->checkOldAge(d);

                // Look for scheduled events this person has today
                for(auto s : it->sched) {
                    if(s->getDate() == d) {
                        s->execute();
                    }
                }
            }
        }

        // Step 2:
        // Check for external events
        if(one_in(c.immigrationFrequency))  // 1 in 3000
            lookForImmigrants(d);      // TODO: schedule immigrants / migration events??!?!?!
        
        finishedForTheDay = true;
    }
}

void simulate()
{
    Date startDate = Date(c.yearZero + 10, 1, 1);
    int years = c.simulationYears;
    stat.start = startDate;

    for(int i = 0; i < (365 * years); i++) {
        ++startDate;
        processDay(startDate);
    }

    stat.end = startDate;
}

int main(int argc, char *argv[])
{
    seed = time(0);

    if(argc > 1) {
        seed = atoi(argv[1]);
    }

	rng.seed(seed);

    readConfigFiles();
	n = new NameGenerator();

    cout << endl << endl << "     SEED: " << seed;
    cout << endl << endl << " P O P U L A T I O N   G E N E R A T O R " << endl << endl;

    setupInitialPopulation();
    simulate();

    // idea: output to textfile! name yyyymmddhhmmss or something.
    for(auto it : pop.getAll()) {
        it->describe(stat.end);
    }

    printStatistics(stat);

	return 0;
}

// vim: fdm=syntax ft=cpp
