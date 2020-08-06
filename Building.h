#ifndef _BUILDING_H_
#define _BUILDING_H_
#include "Square.h"
#include "Subject.h"
#include "Observer.h"
#include <map>
#include <vector>

class Building : public Square, public Subject, public Observer {
    std::string owner;
    int purchaseCost;
    bool isMortgaged;
    std::map<int, std::string> neighbour;
    // A map from the neighbour's location to its owner,
    // where neighbour refers to those in the same monopoly block

  protected:
    int numNeighbourOwned() const;
    bool allNeighbourOwned() const;

  public:
    Building(const std::string& name, int location,
            const std::string& owner, int purchaseCost,
            bool isMortgaged, const std::vector<int>& neighbourIndice);
    virtual void notify(Subject& whoNotified) override;
    virtual int getFee() const = 0;
    std::string getOwner() const;
    void setOwner(std::string owner);
    int getPurchaseCost() const;
    bool getIsMortgaged() const;
    void setIsMortgaged(bool isMortgaged);
};

#endif

