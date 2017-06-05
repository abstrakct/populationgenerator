
#include "namegenerator.h"
#include "entity.h"
#include "person.h"
#include "utils.h"

#include <iostream>
using namespace std;

// Boost random number library
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

int seed;
boost::random::mt19937 rng;

int main()
{
	seed = time(0);
	rng.seed(seed);

	NameGenerator *n = new NameGenerator();

	Person *p = new Person();

	for(int i = 0; i < 10; ++i) {
		p->setName(n->generateName(true));
		p->setBirthYear(1969 + ri(-5, 5));
		cout << p->getName() << " was born in " << p->getBirthYear() << endl;
	}

	delete n;
	delete p;

	return 0;
}
