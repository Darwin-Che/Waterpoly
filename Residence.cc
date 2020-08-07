#include "Residence.h"

Residence::Residence(const std::string& name, int location,
        const std::string& description,
        int purchaseCost, bool isMortgaged)
    : Building(name, location, description, purchaseCost, isMortgaged) {}

