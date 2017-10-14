//
// Class for dealing with dates.
// Can probably be done much more elegantly with modern C++
//

#pragma once

#include <string>

class Date {
    private:
        int year, month, day;
    public:
        Date() : year(0), month(0), day(0) {};
        Date(int y, int m, int d) : year(y), month(m), day(d) {};

        int getYear() { return year; };
        int getMonth() { return month; };
        int getDay() { return day; };

        void incYear(int a = 1);
        void incMonth(int a = 1);
        void incDay(int a = 1);

        Date operator++() {
            incDay();
            return Date(year, month, day);
        };
        
        Date& operator+=(const int& rhs) {
            for(int i = 0; i < rhs; i++)
                incDay();
            return *this;
        };

        friend Date operator+(Date lhs, const int& rhs) {
            lhs += rhs;
            return lhs;
        };

        inline bool operator<(Date rhs) {
            if(this->year < rhs.year)
                return true;
            if(this->year == rhs.year) {
                if(this->month < rhs.month)
                    return true;
                if(this->month == rhs.month) {
                    if(this->day <= rhs.day)
                        return true;
                }
            }

            return false;
        };

        /*friend bool operator<=(Date lhs, Date rhs) {
            if(lhs.year <= rhs.year && lhs.month <= rhs.month && lhs.day <= rhs.day)
                return true;
            else
                return false;
        };*/

        friend bool operator==(Date lhs, Date rhs) {
            if(lhs.year == rhs.year && lhs.month == rhs.month && lhs.day == rhs.day)
                return true;
            else
                return false;
        };
        
        std::string pp();  // pretty print
};


// vim: fdm=syntax ft=cpp
