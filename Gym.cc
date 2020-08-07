#include "Gym.h"

Gym::Gym(const std::string& name, int location,
        const std::string& description,
        int purchaseCost, bool isMortgaged)
    : Building(name, location, description, purchaseCost, isMortgaged) {}

std::string Gym::getInfo() const 
{
    return getName() + "(Gym)"
                     + " ; Mortgaged - " + (getIsMortgaged() ? "true" : "false")
                     + " ; Cost Information: \n"
                     + " \t Purchase Cost: " + std::to_string(getPurchaseCost())
                     + " \t Tuition with owning more: " 
                     + " one - four times dice; two - ten times dice"
                     + " \n";
}
