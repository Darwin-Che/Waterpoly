#ifndef _BUILDING_H_
#define _BUILDING_H_
#include "Square.h"

class Building : public Square {
    int purchaseCost;
    bool isMortgaged;

  public:
    Building(const std::string& name, int location, const std::string &description,
        int purchaseCost, bool isMortgaged);
    int getPurchaseCost() const;
    bool getIsMortgaged() const;
    void setIsMortgaged(bool isMortgaged);
    virtual int calculateNetworth() const;
    virtual std::string getInfo() const;
    virtual std::string saveInfo() const;
};

#endif

