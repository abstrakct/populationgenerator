#pragma once

// Personal events - things that happen in the life of a person
//

#include <string>
#include <memory>
#include "person.h"

class Person;

enum eventType {
    etBirth,
    etMarriage,
    etDeath
};

class PersonalEvent {
    private:
        int year;    // TODO: months, days
        eventType type;
    public:
        std::shared_ptr<Person> owner;

        PersonalEvent(std::shared_ptr<Person> o) {
            owner = o;
        };
        PersonalEvent(std::shared_ptr<Person> o, int date) {
            owner = o;
            year = date;
        };
        PersonalEvent(std::shared_ptr<Person> o, int date, eventType t) {
            owner = o;
            year = date;
            type = t;
        };

        void setDate(int y) { year = y; };
        int  getDate() { return year; };
        eventType getType() { return type; };

        virtual std::string describe() = 0;
        virtual void execute() = 0;
};

class BirthEvent : public PersonalEvent {
    private:

    public:
        //BirthEvent(std::shared_ptr<Person> o) : PersonalEvent(o) {};
        BirthEvent(std::shared_ptr<Person> o, int d) : PersonalEvent(o, d, etBirth) {};

        std::string describe();
        void execute();
};

class MarriageEvent : public PersonalEvent {
    private:

    public:
        MarriageEvent(std::shared_ptr<Person> o, int d) : PersonalEvent(o, d, etMarriage) {};

        std::string describe();
        void execute();
};
