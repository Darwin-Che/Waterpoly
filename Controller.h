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
    std::unique_ptr<cState> cstate;
    std::vector<std::string> playerList;
    int cur;
    int seed;

public:
    Controller(std::shared_ptr<Model> t_model);
    void takeTurn(std::istream &in);
};

#endif
