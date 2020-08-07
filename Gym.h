#ifndef _GYM_H_
#define _GYM_H_
#include "Building.h"

class Gym : public Building {
public:
    Gym(const std::string& name, int location,
        const std::string& description,
        int purchaseCost, bool isMortgaged);
    virtual std::string getInfo() const;
};

#endif

