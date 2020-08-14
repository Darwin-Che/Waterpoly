#ifndef _MODELEXCEPT_
#define _MODELEXCEPT_

#include <string>

class ModelExcept
{
    std::string message;

public:
    ModelExcept(const std::string &m)
        : message{m}
    {
    }
    std::string getmessage()
    {
        return message;
    }
};

#endif
