
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
    outAlive,
    outPregnancies
};

struct Statistics globalStatistics;
OutputFormat out;

int parseCommandLineOptions(int argc, char **argv)
{
    try {
        po::options_description desc("Allowed options");
        desc.add_options()("alive,a", "output full description, but only those alive at the end of simulation")("births,b", "output only births")("deaths,d", "output only deaths and their causes")("pregnancies,p", "output only pregnancies")("help,h", "show this help message")("seed,s", po::value<long>(), "set seed for RNG")("version,v", "show program version");

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
        if (varmap.count("pregnancies")) {
            out = outPregnancies;
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

void processDay(Date date)
{
    bool finishedForTheDay = false;

    while (!finishedForTheDay) {

        // Step 1:
        // Go through all the people, see if something should be done to anyone...
        for (auto it : population.getAll()) {
            if (it->isAlive()) {
                if (!it->isOrphan() && it->getAge(date) < c.orphanMaxAge) {
                    std::shared_ptr<Person> f = it->getFather();
                    std::shared_ptr<Person> m = it->getMother();
                    if (f && !f->isAlive() && m && !m->isAlive()) {
                        it->makeOrphan(date);
                    }
                }
                if (!it->isMarried() && it->getAge(date) >= c.ageAdult && one_in(c.marriageFrequency)) // 1 in 30
                    lookForPartners(it, date);
                if (one_in(c.sexyTimeFrequency)) // 1 in 5
                    lookForSexyTime(it, date);
                if (one_in(c.unexpectedDeathFrequency)) // 1 in 20000
                    it->deathForVariousReasons(date);
                if (one_in(c.oldAgeDeathFrequency)) // 1 in 200
                    it->checkOldAge(date);

                // Look for scheduled events this person has today
                for (auto s : it->sched) {
                    if (s->getDate() == date) {
                        s->execute();
                    }
                }
            }
        }

        // Step 2:
        // Check for external events
        if (one_in(c.immigrationFrequency)) // 1 in 3000
            lookForImmigrants(date);        // TODO: schedule immigrants / migration events??!?!?!

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

void drawFamilyTree(std::shared_ptr<Person> startPerson, int step)
{
    // if (step == 0) {
    //     std::cout << startPerson->getName();
    //     if (startPerson->isMarried()) {
    //         cout << " - " << startPerson->getSpouse()->getName() << std::endl;
    //     } else {
    //         cout << std::endl;
    //     }
    //     if (startPerson->getChildren().size() > 0) {
    //         for (auto it : startPerson->getChildren()) {
    //             drawFamilyTree(it, step + 1);
    //         }
    //     }
    if (step > 0) {
        for (int i = step; i > 0; i--) {
            cout << "    ";
        }
    }

    std::cout << startPerson->getName();
    if (startPerson->isMarried()) {
        cout << " -+- " << startPerson->getSpouse()->getName() << std::endl;
    } else {
        cout << std::endl;
    }
    if (startPerson->getChildren().size() > 0) {
        for (auto it : startPerson->getChildren()) {
            drawFamilyTree(it, step + 1);
        }
    }
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
    if (out == outPregnancies)
        printAllEventsOfType(etPregnant);

    printStatistics(globalStatistics);

    //drawFamilyTree(population.getAll().front(), 0);

    // std::shared_ptr<Person> p;
    // p = population.spawnPerson();
    // p->generateRandom();
    // p->generateGenes();
    // p->setGender(male);
    // printf("Genetic infertility factor: %.1f\n", p->getGenetics().infertility_factor);
    // for (int i = 0; i < 100; i++) {
    //     printf("Age: %02d - Calculated IF: %.1f\n", i, p->getGenetics().getInfertilityFactor(i));
    // }

    // delete log (close file)
    delete l;

    return 0;
}

// vim: fdm=syntax ft=cpp
