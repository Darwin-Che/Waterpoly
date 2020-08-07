#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include "Subject.h"

class Player: public Subject{
    char symbol;
    std::string name;
    int money, position, oldPosition;
    int numCups;
    static int totalNumCups;
    bool isJailed;
    int numJailed;
    int debt;
    std::string debtOwner;
    bool isBankrupt;
  public:
    // constructors
    Player(char symbol, const std::string& name);
    Player(char symbol, const std::string& name, int position, int numCups, int money);
    Player(char symbol, const std::string& name, int position, int numCups, int money, int numJailed);
    // accessors and mutators
    char getSymbol() const;
    std::string getName() const;

    int getMoney() const;
    void setMoney(const int & money);

    int getPosition() const;
    void setPosition(const int & position);

    int getNumCups() const;
    void setNumCups(const int & numcups);

    static int getTotalNumCups();

    bool getIsJailed() const;
    void setIsJailed(const bool & isJaied);

    int getNumJailed() const;
    void setNumJailed(const int & getNumJailed);

    int getDebt() const;
    void setDebt(const int & debt);

    std::string getDebtOwner() const;
    void setDebtOwner(const std::string & name);

    bool getIsBankrupt() const;

    // check if the player has enough money to pay debt
    bool canPayDebt() const;

    // notify Observers when the player bankrupt
    void dropOut();
};

#endif
