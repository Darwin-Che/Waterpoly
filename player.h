#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include "Subject.h"

class Player: public Subject{
    char symbol;
    std::string name;
    int money, position;
    int numCups;
    bool isJailed;
    int numJailed;
    int debt;
    std::string debtOwner;
    bool isBankrupt;
  public:
    // constructor
    Player(char symbol, std::string name);
    // accessors and mutators
    char getSymbol();
    std::string getName();

    int getMoney();
    void setMoney(const int & money);

    int getPosition();
    void setPosition(const int & position);

    int getNumCups();
    void setNumCups(const int & numcups);

    bool getIsJailed();
    void setIsJailed(const bool & isJaied);

    int getNumJailed();
    void setNumJailed(const int & getNumJailed);

    int getDebt();
    void setDebt(const int & debt);

    std::string getDebtOwner();
    void setDebtOwner(const std::string & name);

    bool getIsBankrupt();

    // check if the player has enough money to pay debt
    bool canPayDebt();

    // notify Observers when the player bankrupt
    void dropOut();
};

#endif
