#ifndef _CONTROLLER_
#define _CONTROLLER_

#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include "Model.h"


class cState;

class Controller
{
    std::shared_ptr<Model> model;
    std::shared_ptr<cState> cstate;
    std::string curPlayerName;

public:
    Controller(std::shared_ptr<Model> t_model, const std::string &startPlayerName, bool t_testingMode_roll);
    void takeTurn(std::istream &in);
};

#endif
