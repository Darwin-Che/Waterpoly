#ifndef _BUILDING_H_
#define _BUILDING_H_
#include "Square.h"
#include "Subject.h"

class Building : public Square, public Subject {
    int purchaseCost;
    bool isMortgaged;
  protected:
    char owner;
  public:
    Building(const std::string& name, int location, const std::string &t_description,
        int purchaseCost, bool isMortgaged);
    virtual ~Building() = default;
    int getPurchaseCost() const;
    bool getIsMortgaged() const;
    void setIsMortgaged(bool isMortgaged);
    char setOwner(char o);
    virtual int calculateNetworth() const;
    virtual std::string getInfo() const;
    virtual std::string saveInfo() const;
    virtual Info getSubjectInfo();
};

#endif

