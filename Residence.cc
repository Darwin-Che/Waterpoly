#include "Residence.h"

Residence::Residence(const std::string &name, int location,
                     const std::string &t_description,
                     int purchaseCost, bool isMortgaged)
    : Building(name, location, t_description, purchaseCost, isMortgaged) {
        description = "Residence";
    }

std::string Residence::getInfo() const
{
    return Building::getInfo() +
           + "\n\tPurchase Cost: " + std::to_string(getPurchaseCost()) 
           + "\n\tTuition with owning more: " + std::to_string(25) 
           + " " + std::to_string(50) 
           + " " + std::to_string(100) 
           + " " + std::to_string(200);
}
