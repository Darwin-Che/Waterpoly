#include "Gym.h"

Gym::Gym(const std::string &name, int location,
         const std::string &t_description,
         int purchaseCost, bool isMortgaged)
    : Building(name, location, t_description, purchaseCost, isMortgaged) {
        description = "Gym";
    }

std::string Gym::getInfo() const
{
    return Building::getInfo() +
           + "\n\tPurchase Cost: " + std::to_string(getPurchaseCost()) 
           + "\n\tTuition with owning more: " + " one - four times dice; two - ten times dice ";
}
