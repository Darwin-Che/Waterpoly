#include "ModelImplHouseRules.h"
#include "Board.h"
#include "AcademicBuilding.h"

// Option 1, 2 and 3 are the 3 house rules mentioned in watopoly.pdf
// Option 4 is the "even build" rule in the classic Monopoly
ModelImplHouseRules::ModelImplHouseRules(std::istream &tin, std::ostream &tout, std::vector<int> houseRuleOptions)
    : ModelImplDefault(tin, tout), amountCollected(0) {
    
    for (int option : houseRuleOptions) {
        if (option == 1) option12Activated = true;
        if (option == 2) {
            option12Activated = true;
            amountCollected = 500;
        }
        if (option == 3) option3Activated = true;
        if (option == 4) option4Activated = true;
    }
}

void ModelImplHouseRules::applyStrategy(std::shared_ptr<Player> p) {
    std::string squareName = board->getSquare(p->getPosition())->getName();
    if (option12Activated && (squareName == "TUITION" ||
                squareName == "COOP FEE" || squareName == "DC Tims Line")) {
        int moneyBefore = p->getMoney();
        ModelImplPrimitive::applyStrategy(p);
        int moneyAfter = p->getMoney();
        if (moneyAfter < moneyBefore) {
            amountCollected += moneyBefore - moneyAfter;
            show("Due to the house rule, these money will go to the centre of board.");
        }

    } else {
        ModelImplPrimitive::applyStrategy(p);
        if (option12Activated && squareName == "Goose Nesting") {
            p->setMoney(p->getMoney() + amountCollected);
            show("Mr. Goose brings you fortune! You received $" +
                    std::to_string(amountCollected) + "!");
            amountCollected = 0;
        }

        if (option3Activated && squareName == "COLLECT OSAP") {
            p->setMoney(p->getMoney() + 200);
            show("Due to the house rule, you received an additional $200!");
        }
    }
}

bool ModelImplHouseRules::squareImprovable(const std::string &pn, const std::string &property) {
    std::shared_ptr<Square> s = board->getSquareBuilding(property);
    bool result = ModelImplDefault::squareImprovable(pn, property);
    if (result == false) return false;

    std::shared_ptr<AcademicBuilding> ptr = std::dynamic_pointer_cast<AcademicBuilding>(s);
    std::vector<std::shared_ptr<Square>> monopoly = board->getMonopoly(ptr->getName());
    int curLevel = ptr->getImprovementLevel();
    
    for (auto neighbour : monopoly) {
        std::shared_ptr<AcademicBuilding> nbPtr =
            std::dynamic_pointer_cast<AcademicBuilding>(neighbour);
        int nbLevel = nbPtr->getImprovementLevel();
        if (nbLevel != curLevel && nbLevel != curLevel + 1) {
            show("Improvement failed. Due to the even build rule, the improvement levels of all buildings in your monopoly can only be differ by 1.");
            return false;
        }
    }
    return true;
}

