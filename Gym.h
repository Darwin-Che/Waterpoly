#ifndef _GYM_H_
#define _GYM_H_
#include "Building.h"

class Gym : public Building {
  public:
    Gym(const std::string name, int location,
            std::shared_ptr<VisitStrategy> visitStrategy,
            const std::string owner, int purchaseCost,
            bool isMortgaged, const std::vector<int>& neighbourIndice);
    virtual int getFee() const override; 
};

#endif

