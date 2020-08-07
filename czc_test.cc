#include "Building.h"
#include "AcademicBuilding.h"
#include "Square.h"
#include "Gym.h"
#include "Residence.h"
#include <memory>
#include <vector>
#include <iostream>
#include <string>

int main() {
    std::shared_ptr<Square> s = std::make_shared<Square>("square 0", 0, "square 0");
    std::shared_ptr<Building> b = std::make_shared<Building>("bulding 1", 1, "building 1", 100, false);
    std::vector<int> i;
    i.emplace_back(1);
    i.emplace_back(2);
    i.emplace_back(3);
    i.emplace_back(4);
    std::shared_ptr<AcademicBuilding> ab =  std::make_shared<AcademicBuilding>("abbuilding 2", 2, "abbuilding 2", 100, false, 50, i);
    std::shared_ptr<Residence> g = std::make_shared<Residence>("gym 3", 3, "gym", 100, false);

    g->setIsMortgaged(true);
    std::cout << ab->getInfo() << std::endl;
}