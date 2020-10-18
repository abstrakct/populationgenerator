
#include "config.h"
#include "date.h"
#include "entity.h"
#include "logger.h"
#include "name.h"
#include "person.h"
#include "population.h"
#include "utils.h"
#include "version.h"

#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <sstream>
#include <vector>

using namespace std;

// Boost random number library
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
// Boos command line options parser
#include <boost/program_options.hpp>
namespace po = boost::program_options;

long seed;
boost::random::mt19937 rng;
NameGenerator *n;
Population population;
ConfigData c;
// TODO: implement logger class properly!!
Logger *l;

enum OutputFormat {
    outVersion,
    outEverything,
    outDeaths,
    outBirths,
    outAlive
};

struct Statistics globalStatistics;
OutputFormat out;

int parseCommandLineOptions(int argc, char **argv)
{
    try {
        po::options_description desc("Allowed options");
        desc.add_options()("alive,a", "output full description, but only those alive at the end of simulation")("births,b", "output only births")("deaths,d", "output only deaths and their causes")("help,h", "show this help message")("seed,s", po::value<long>(), "set seed for RNG")("version,v", "show program version");

        po::variables_map varmap;
        po::store(po::parse_command_line(argc, argv, desc), varmap);
        po::notify(varmap);

        if (varmap.count("help")) {
            cout << desc << endl;
            return 2;
        }

        if (varmap.count("seed")) {
            seed = varmap["seed"].as<long>();
        }

        if (varmap.count("version")) {
            out = outVersion;
        }

        if (varmap.count("deaths")) {
            out = outDeaths;
        }
        if (varmap.count("births")) {
            out = outBirths;
        }
        if (varmap.count("alive")) {
            out = outAlive;
        }
    } catch (exception &e) {
        cerr << "error: " << e.what() << endl;
        return 1;
    } catch (...) {
        cerr << "Exception of unknown type!" << endl;
    }

    return 0;
}

void setupInitialPopulation()
{
    int num;

    num = ri(c.initialPopulationMin, c.initialPopulationMax);

    for (int i = 0; i < num; ++i) {
        std::shared_ptr<Person> p;

        p = population.spawnPerson();
        p->generateRandom();
        p->setBornHere(true);
    }

    // TODO: add some that are already couples?
    globalStatistics.initialPopulation = num;
    globalStatistics.totalNumberOfPeople = num;
}

void lookForImmigrants(Date d)
{
    std::shared_ptr<Person> p;
    p = population.spawnPerson();

    int birthyear = d.getYear() + ri(-38, -18); // immigrants are between 18 - 38 years old
    int birthmonth = ri(1, 12);
    int birthday = ri(1, 28);
    Date bd = Date(birthyear, birthmonth, birthday);

    p->generateRandom(bd);
    p->setBornHere(false);
    MigrationEvent *mig;
    mig = new MigrationEvent(p, d);
    p->ev.push_back(mig);
    globalStatistics.immigrants++;
    globalStatistics.totalNumberOfPeople++;
}

void processDay(Date d)
{
    bool finishedForTheDay = false;

    while (!finishedForTheDay) {

        // Step 1:
        // Go through all the people, see if something should be done to anyone...
        for (auto it : population.getAll()) {
            if (it->isAlive()) {
                if (!it->isMarried() && it->getAge(d) >= c.ageAdult && one_in(c.marriageFrequency)) // 1 in 30
                    lookForPartners(it, d);
                if (one_in(c.sexyTimeFrequency)) // 1 in 5
                    lookForSexyTime(it, d);
                if (one_in(c.unexpectedDeathFrequency)) // 1 in 20000
                    it->deathForVariousReasons(d);
                if (one_in(c.oldAgeDeathFrequency)) // 1 in 200
                    it->checkOldAge(d);

                // Look for scheduled events this person has today
                for (auto s : it->sched) {
                    if (s->getDate() == d) {
                        s->execute();
                    }
                }
            }
        }

        // Step 2:
        // Check for external events
        if (one_in(c.immigrationFrequency)) // 1 in 3000
            lookForImmigrants(d);           // TODO: schedule immigrants / migration events??!?!?!

        finishedForTheDay = true;
    }
}

void simulate()
{
    Date startDate = Date(c.yearZero + 10, 1, 1);
    int years = c.simulationYears;
    globalStatistics.start = startDate;

    for (int i = 0; i < (365 * years); i++) {
        ++startDate;
        processDay(startDate);
    }

    globalStatistics.end = startDate;
}

void printAllEventsOfType(eventType t)
{
    std::list<PersonalEvent *> thelist;

    for (auto it : population.getAll()) {
        for (auto e : it->ev) {
            if (e->getType() == t) {
                thelist.push_back(e);
            }
        }
    }

    thelist.sort([](PersonalEvent *a, PersonalEvent *b) { return a->getDate() < b->getDate(); }); // LAMBDA!
    for (auto it : thelist)
        cout << it->describe();
}

int main(int argc, char **argv)
{
    int parseResult;
    std::stringstream outfilename;

    seed = time(0);
    out = outEverything;

    parseResult = parseCommandLineOptions(argc, argv);
    if (parseResult == 1) {
        cerr << "parsing command line failed!" << endl;
        return 1;
    } else if (parseResult == 2) {
        return 0;
    }

    if (out == outVersion) {
        cout << "Population Generator v" << VERSION_STRING << endl;
        exit(0);
    }

    readConfigFiles();

    rng.seed(seed);
    outfilename << "logs/" << seed << ".txt";
    l = new Logger(outfilename.str());

    n = new NameGenerator();

    cout << endl
         << endl
         << "     SEED: " << seed;
    cout << endl
         << endl
         << " P O P U L A T I O N   G E N E R A T O R     v" << VERSION_STRING << endl
         << endl;

    setupInitialPopulation();
    simulate();

    // idea: output to textfile! name yyyymmddhhmmss or something.
    if (out == outEverything) {
        for (auto it : population.getAll()) {
            it->describe(globalStatistics.end, true);
        }
    }

    if (out == outAlive) {
        for (auto it : population.getAllAlive()) {
            it->describe(globalStatistics.end, true);
        }
    }

    if (out == outDeaths)
        printAllEventsOfType(etDeath);
    if (out == outBirths)
        printAllEventsOfType(etBirth);

    printStatistics(globalStatistics);

    // delete log (close file)
    delete l;

    return 0;
}

// vim: fdm=syntax ft=cpp
