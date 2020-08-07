#include "ResidenceStrategy.h"
#include "Building.h"

void ResidenceStrategy::acceptVisitor(std::shared_ptr<Player> player,
        std::shared_ptr<Board> board, std::istream in, std::ostream out) {
    std::shared_ptr<Square> square = board->getSquare(player->getPosition());
    std::shared_ptr<Building> building = dynamic_pointer_cast<Building>(square);

    if (building != nullptr) {
        std::string buildingName = building->getName();
        std::shared_ptr<Player> owner = board->getOwner(buildingName);
        out << "You stepped on Residence " << buildingName << "." << std::endl;
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

        int numOwned = board->numNeighbourOwned(buildingName);
        int fee = 0;
        if (numOwned == 1)
            fee = 25;
        else if (numOwned == 2)
            fee = 50;
        else if (numOwned == 3)
            fee = 100;
        else
            fee = 200;

        if (player->getMoney() >= fee) {
            player->setMoney(player->getMoney() - fee);
            owner->setMoney(owner->getMoney() + fee);
        } else {
            player->setDebt(fee);
            player->setDebtOwner(owner->getName());
        }
    }
}

