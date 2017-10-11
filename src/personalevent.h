#pragma once

// Personal events - things that happen in the life of a person
//

#include <string>
#include <memory>
#include "person.h"
#include "date.h"

class Person;

enum eventType {
    etBirth,
    etMarriage,
    etDeath
};

class PersonalEvent {
    private:
        Date date;
        eventType type;
    public:
        std::shared_ptr<Person> owner;

        PersonalEvent(std::shared_ptr<Person> o) {
            owner = o;
        };
        PersonalEvent(std::shared_ptr<Person> o, Date d) {
            owner = o;
            date = d;
        };
        PersonalEvent(std::shared_ptr<Person> o, Date d, eventType t) {
            owner = o;
            date = d;
            type = t;
        };

        void setDate(Date d) { date = d; };
        Date getDate() { return date; };
        eventType getType() { return type; };

        virtual std::string describe() = 0;
        virtual void execute() = 0;
};

class BirthEvent : public PersonalEvent {
    private:

    public:
        //BirthEvent(std::shared_ptr<Person> o) : PersonalEvent(o) {};
        BirthEvent(std::shared_ptr<Person> o, Date d) : PersonalEvent(o, d, etBirth) {};

        std::string describe();
        void execute();
};

class MarriageEvent : public PersonalEvent {
    private:

    public:
        MarriageEvent(std::shared_ptr<Person> o, Date d) : PersonalEvent(o, d, etMarriage) {};

        std::string describe();
        void execute();
};
