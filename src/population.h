/*
 * Population class
 */

#pragma once

#include "person.h"
#include <list>
#include <vector>

class Population
{
private:
    std::vector<std::shared_ptr<Person>> pop;

public:
    void addPerson(std::shared_ptr<Person> p);
    std::shared_ptr<Person> spawnPerson();

    std::vector<std::shared_ptr<Person>> getAll();
    std::vector<std::shared_ptr<Person>> getAllUnmarried();
    std::vector<std::shared_ptr<Person>> getAllUnmarried(Date d);
    std::vector<std::shared_ptr<Person>> getAllAlive();
    std::vector<std::shared_ptr<Person>> getAllDead();
    std::list<std::shared_ptr<Person>> getAllSortedList();
};

// vim: fdm=syntax ft=cpp
