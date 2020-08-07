#ifndef _RESIDENCE_H_
#define _RESIDENCE_H_
#include "Building.h"

class Residence : public Building {
public:
    Residence(const std::string& name, int location,
        const std::string& description,
        int purchaseCost, bool isMortgaged);
    virtual std::string getInfo() const;
};

#endif

