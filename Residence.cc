#include "Residence.h"

Residence::Residence(const std::string& name, int location,
        const std::string& description,
        int purchaseCost, bool isMortgaged)
    : Building(name, location, description, purchaseCost, isMortgaged) {}

std::string Residence::getInfo() const 
{
    return getName() + "(Residence)"
                     + " ; Mortgaged - " + (getIsMortgaged() ? "true" : "false")
                     + " ; Cost Information: \n"
                     + " \t Purchase Cost: " + std::to_string(getPurchaseCost())
                     + " \t Tuition with owning more: " + std::to_string(25) 
                     + " " + std::to_string(50)
                     + " " + std::to_string(100)
                     + " " + std::to_string(200)
                     + " \n";
}
