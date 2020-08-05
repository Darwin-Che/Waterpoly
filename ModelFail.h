#ifndef _MODELFAIL_
#define _MODELFAIL_

#include <string>

class ModelFail
{
    std::string message;

public:
    ModelFail(const std::string &m)
        : message{m}
    {
    }
    std::string getmessage()
    {
        return message;
    }
};

#endif
