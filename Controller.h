#ifndef _CONTROLLER_
#define _CONTROLLER_

#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include "Model.h"
#include "ModelFail.h"

class cState;

class Controller
{
    std::shared_ptr<Model> model;
    std::unique_ptr<cState> cstate;
    std::vector<std::string> playerList;
    int cur;
    int seed;

public:
    void takeTurn(std::istream in);
};

#endif
