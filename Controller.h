#ifndef _CONTROLLER_
#define _CONTROLLER_

#include <vector>
#include <string>
#include <iostream>
#include <memory>

class cState; 

class Controller
{
    Model model;
    std::unique_ptr<cState> cstate;
    std::vector<std::string> playerList;
    int cur;
    int seed;
    

public:
    void takeTurn(std::istream in);
};

#endif
