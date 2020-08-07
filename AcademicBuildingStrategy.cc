#include "AcademicBuildingStrategy.h"
#include "AcademicBuilding.h"

// The player will pay tuition to the owner if applicable
void AcademicBuildingStrategy::acceptVisitor(std::shared_ptr<Player> player,
        std::shared_ptr<Board> board, std::istream& in, std::ostream& out) {
    std::shared_ptr<Square> square = board->getSquare(player->getPosition());
    std::shared_ptr<AcademicBuilding> building =
        std::dynamic_pointer_cast<AcademicBuilding>(square);

    if (building != nullptr) {
        std::string buildingName = building->getName();
        std::shared_ptr<Player> owner = board->getOwner(buildingName);
        out << "You stepped on Academic Building " << buildingName << "." << std::endl;

        // No fee is paid if the building is mortgaged or if the owner landed on it
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

        // Get the amount of tuition corresponding to the improvement level
        int fee = building->getImprovementFee();
        // Double tuition if owner controls the monopoly and no improvement is done
        if (board->inMonopoly(buildingName) && building->getImprovementLevel() == 0)
            fee = fee * 2;
        if (player->getMoney() >= fee) {
            player->setMoney(player->getMoney() - fee);
            owner->setMoney(owner->getMoney() + fee);
        } else {
            player->setDebt(fee);
            player->setDebtOwner(owner->getName());
        }
        out << "You paid $" << fee << " tuition to the property's owner "
            << owner->getName() << "." << std::endl;
    }
}

