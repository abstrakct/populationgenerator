
#include <string>
#include <sstream>
#include "personalevent.h"

std::string BirthEvent::describe()
{
    std::stringstream s;

    s << owner->getName() << " was born in " << getDate() << "." << std::endl;

    std::string ret = s.str();
    return ret;
}

void BirthEvent::execute()
{
}
