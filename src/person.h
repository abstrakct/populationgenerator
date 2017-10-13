#pragma once

#include "entity.h"
#include "personalevent.h"
#include "date.h"

#include <vector>
#include <memory>

// Forward declarations
class PersonalEvent;
class Person;
void lookForPartners(shared_ptr<Person> p, Date d);
void lookForSexyTime(shared_ptr<Person> p, Date d);

enum Gender {
    male,
    female,
    other
};

class Person : public std::enable_shared_from_this<Person> {
	private:
	    Name name;
		Gender gender;
		bool bornHere;
		int movedYear;
		bool married, pregnant = false;
		bool alive = true;

        std::shared_ptr<Person> mother = NULL;
        std::shared_ptr<Person> father = NULL;
        std::shared_ptr<Person> spouse = NULL;
        std::vector<Person*> siblings;

        
	public:

        std::vector<PersonalEvent*> ev;         // TODO: change to map (?) for easier lookup!!??? wouldn't work for multiple marriages etc.

		std::string getName() { return name.get(); };
		void setName(Name n) { name.set(n); };
        std::string getFamilyName() { return name.getFamily(); };
        void setAlive(bool b = true) { alive = b; };
        bool isAlive() { return alive; };
		void setGender(Gender g) { gender = g; };
		Gender getGender() { return gender; };
		void setBornHere(bool b) { bornHere = b; };
		bool getBornHere() { return bornHere; };
		void setMovedYear(int y) { movedYear = y; };
		int  getMovedYear() { return movedYear; };
		void setMarried(bool b) { married = b; };
		bool isMarried() { return married; };
		void setPregnant(bool b) { pregnant = b; };
		bool isPregnant() { return pregnant; };
		void setSpouse(std::shared_ptr<Person> p) { spouse = p; };
        std::shared_ptr<Person> getSpouse() { return spouse; };
        void makeWidow(Date d);
        Date getWidowDate();
        Date getDeathDate();

        std::string getPossessivePronoun() { 
            if(gender == male)
                return "his";
            else if(gender == female)
                return "her";
            else
                return "its";
        };

        std::string getPersonalPronoun() { 
            if(gender == male)
                return "he";
            else if(gender == female)
                return "she";
            else
                return "it";
        };

        int getAge(int year) {
            return (year - getBirthYear());
        };

        int getAge(Date d);

		void describe();
		void generateRandom();
		int  getBirthYear();
		Date getBirthday();
		int  getMarriageYear();
        Date getMarriageDate();
        void fuck(std::shared_ptr<Person> partner, Date d);
		void marry(std::shared_ptr<Person> spouse, Date date);
		void kill(Date d);
		void impregnate(Date d);

		// Checks for various things
		void checkOldAge(Date d);
		void checkUnexpectedDeath(Date d);
};
