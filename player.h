#ifndef PLAYER_H
#define PLAYER_H
#include <vector>
#include <map>
#include <string>
#include "subject.h"

class Player: public Subject{
    char symbol;
    std::string name;
    int money, position;
    int numCups;
    bool isJailed;
    int numJailed;
  public:
    
};

#endif
