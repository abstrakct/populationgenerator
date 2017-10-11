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
        
        std::string pp();  // pretty print
};
