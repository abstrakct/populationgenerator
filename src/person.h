#pragma once

#include "entity.h"
#include <vector>
#include <memory>

enum Gender {
    male,
    female,
    other
};


class Person : public Entity {
	private:
		int birthYear;
		Gender gender;
		bool bornHere;
		int movedYear;
		bool married;
		int marriedYear;

        std::shared_ptr<Person> mother = NULL;
        std::shared_ptr<Person> father = NULL;
        std::shared_ptr<Person> spouse;
        std::vector<Person*> siblings;

        
	public:

		void setBirthYear(int y) { birthYear = y; };
		int  getBirthYear() { return birthYear; };
		void setGender(Gender g) { gender = g; };
		Gender getGender() { return gender; };
		void setBornHere(bool b) { bornHere = b; };
		bool getBornHere() { return bornHere; };
		void setMovedYear(int y) { movedYear = y; };
		int  getMovedYear() { return movedYear; };
		void setMarried(bool b) { married = b; };
		bool isMarried() { return married; };
		void setSpouse(std::shared_ptr<Person> p) { spouse = p; };
		void setMarriedYear(int y) { marriedYear = y; };
		int  getMarriedYear() { return marriedYear; };

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
            return (year - birthYear);
        };

		void describe();
		void generateRandom();
};
