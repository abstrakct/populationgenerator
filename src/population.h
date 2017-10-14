/*
 * Population class
 */

#pragma once

#include <vector>
#include "person.h"


class Population {
    private:
        std::vector<std::shared_ptr<Person>> pop;
    public:
        void addPerson(std::shared_ptr<Person> p);
        std::shared_ptr<Person> spawnPerson();

        std::vector<std::shared_ptr<Person>> getAll();
        std::vector<std::shared_ptr<Person>> getAllUnmarried();
        std::vector<std::shared_ptr<Person>> getAllDead();
};

// vim: fdm=syntax ft=cpp
