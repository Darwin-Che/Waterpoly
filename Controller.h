#ifndef _CONTROLLER_
#define _CONTROLLER_

#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include "Model.h"

class Controller
{
    std::shared_ptr<Model> model;
    std::string curPlayerName;

public:
    Controller(std::shared_ptr<Model> t_model, const std::string &startPlayerName);
    void takeTurn(std::istream &in);
};

#endif
