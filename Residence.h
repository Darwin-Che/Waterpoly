#ifndef _RESIDENCE_H_
#define _RESIDENCE_H_
#include "Building.h"

class Residence : public Building {
  public:
    Residence(const std::string name, int location,
            std::shared_ptr<VisitStrategy> visitStrategy,
            const std::string owner, int purchaseCost,
            bool isMortgaged, const std::vector<int>& neighbourIndice);
    virtual int getFee() const override; 
};

#endif

