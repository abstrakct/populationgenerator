#pragma once

/*
 * Logger class - for logging stuff to a text file.
 */

#include <string>
#include <iostream>
#include <fstream>

class Logger {
    private:
        std::ofstream logfile;

    public:
        Logger() {}
        Logger(std::string filename)
        {
            logfile.open(filename);
        }
        ~Logger()
        {
            logfile.close();
        }

        void w(std::string text)
        {
            logfile << text;
        }

        template <typename T>
            const Logger& operator<<(const T& data)
            {
                logfile << data;
                return (*this);
            }
};
