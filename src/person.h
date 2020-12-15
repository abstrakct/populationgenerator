#pragma once

#include "date.h"
#include "entity.h"
#include "p-event.h"

#include <map>
#include <memory>
#include <vector>

// Forward declarations
class PersonalEvent;
class Person;
void lookForPartners(std::shared_ptr<Person> p, Date d);
void lookForSexyTime(std::shared_ptr<Person> p, Date d);

enum Gender {
    male,
    female,
    unknown,
    other
};

class Person : public std::enable_shared_from_this<Person>
{
private:
    Name name;
    Gender gender;
    bool bornHere;
    bool married, pregnant = false;
    bool alive = true;

    std::shared_ptr<Person> mother = nullptr;
    std::shared_ptr<Person> father = nullptr;
    std::shared_ptr<Person> spouse = nullptr; // current spouse
    std::vector<Person *> siblings;
    std::vector<std::shared_ptr<Person>> spouses;

    // Statistics
    struct {
        int marriages = 0;
        int sexytimes = 0;
        int pregnancies = 0;
    } statistics;

public:
    std::vector<PersonalEvent *> ev; // TODO: shared_ptr?? // TODO: change to map (?) for easier lookup!!??? wouldn't work for multiple marriages etc.
    std::vector<PersonalEvent *> sched;
    //std::map<Date, PersonalEvent*> sched;   // Scheduled events! mapped by date

    Person() = default;

    std::string getName();
    void setName(Name n) { name.set(n); };
    std::string getFamilyName() { return name.getFamily(); };
    std::string getMaidenName() { return name.getMaidenName(); };
    std::string getGivenName() { return name.getGiven(); };
    void setAlive(bool b = true) { alive = b; };
    bool isAlive() { return alive; };
    bool isAlive(Date d)
    {
        if (getDeathDate() == Date(0, 0, 0)) // person isn't dead!
            return true;
        return (d < getDeathDate());
    };
    void setGender(Gender g) { gender = g; };
    Gender getGender() { return gender; };
    void setBornHere(bool b) { bornHere = b; };
    bool getBornHere() { return bornHere; };
    //void setMovedYear(int y) { movedYear = y; };
    //int  getMovedYear() { return movedYear; };
    void setMarried(bool b) { married = b; };
    bool isMarried() { return married; };
    void setPregnant(bool b) { pregnant = b; };
    bool isPregnant() { return pregnant; };
    void setSpouse(std::shared_ptr<Person> p) { spouse = p; };
    void setParents(std::shared_ptr<Person> mo, std::shared_ptr<Person> fa)
    {
        mother = mo;
        father = fa;
    };
    std::shared_ptr<Person> getSpouse()
    {
        if (spouse != nullptr)
            return spouse;
        else
            return spouses.back();
    };
    std::shared_ptr<Person> getLastSpouse() { return spouses.back(); };
    std::vector<std::shared_ptr<Person>> getAllSpouses() { return spouses; };
    void makeWidow(Date d);
    Date getWidowDate();
    Date getDeathDate();
    std::shared_ptr<Person> giveBirth(Date d);

    std::string getPossessivePronoun()
    {
        if (gender == male)
            return "his";
        else if (gender == female)
            return "her";
        else if (gender == unknown)
            return "their";
        else
            return "its";
    };

    std::string getPersonalPronoun()
    {
        if (gender == male)
            return "he";
        else if (gender == female)
            return "she";
        else if (gender == unknown)
            return "they";
        else
            return "it";
    };

    int getAge(int year)
    {
        return (year - getBirthYear());
    };

    int getAge(Date d);

    void describe(Date d = Date(0, 0, 0), bool stats = false);
    void generateRandom();
    void generateRandom(Date bd);
    int getBirthYear();
    Date getBirthday();
    int getMarriageYear();
    Date getMarriageDate();
    void fuck(std::shared_ptr<Person> partner, Date d);
    void marry(std::shared_ptr<Person> spouse, Date date);
    void kill(Date d);
    void kill(Date d, std::string reason);
    void impregnate(Date d);

    // Checks for various things
    void checkOldAge(Date d);
    void deathForVariousReasons(Date d);
};

// vim: fdm=syntax ft=cpp
