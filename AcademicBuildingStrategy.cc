#include "AcademicBuildingStrategy.h"
#include "AcademicBuilding.h"

void AcademicBuildingStrategy::acceptVisitor(std::shared_ptr<Player> player,
        std::shared_ptr<Board> board, std::istream in, std::ostream out) {
    std::shared_ptr<Square> square = board->getSquare(player->getPosition());
    std::shared_ptr<AcademicBuilding> building =
        dynamic_pointer_cast<AcademicBuilding>(square);

    if (building != nullptr) {
        std::string buildingName = building->getName();
        std::shared_ptr<Player> owner = board->getOwner(buildingName);
        out << "You stepped on Academic Building " << buildingName << "." << std::endl;
        if (owner == player) {
            out << "But you are the owner of this property, "
                << "so you do not need to pay any tuition." << std::endl;
            return;
        }
        if (building->getIsMortgaged() == true) {
            out << "But this property is currently mortgaged, "
                << "so you do not need to pay any tuition." << std::endl;
            return;
        }

        int fee = building->getImprovementFee();
        if (board->inMonopoly(buildingName) && building->getImprovementLevel() == 0)
            fee = fee * 2;
        if (player->getMoney() >= fee) {
            player->setMoney(player->getMoney() - fee);
            owner->setMoney(owner->getMoney() + fee);
        } else {
            player->setDebt(fee);
            player->setDebtOwner(owner->getName());
        }
    }
}

