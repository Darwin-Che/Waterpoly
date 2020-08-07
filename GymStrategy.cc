#include "GymStrategy.h"
#include "Building.h"
#include <cstdlib>

void GymStrategy::acceptVisitor(std::shared_ptr<Player> player,
        std::shared_ptr<Board> board, std::istream in, std::ostream out) {
    std::shared_ptr<Square> square = board->getSquare(player->getPosition());
    std::shared_ptr<Building> building = dynamic_pointer_cast<Building>(square);

    if (building != nullptr) {
        std::string buildingName = building->getName();
        std::shared_ptr<Player> owner = board->getOwner(buildingName);
        out << "You stepped on Gym " << buildingName << "." << std::endl;
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

        int roll1 = rand() % 6;
        if (roll1 == 0)
            roll1 = 6;
        int roll2 = rand() % 6;
        if (roll2 == 0)
            roll2 = 6;
        int numOwned = board->numNeighbourOwned(buildingName);
        int fee = 0;
        if (numOwned == 1)
            fee = 4 * (roll1 + roll2);
        else
            fee = 10 * (roll1 + roll2);

        if (player->getMoney() >= fee) {
            player->setMoney(player->getMoney() - fee);
            owner->setMoney(owner->getMoney() + fee);
        } else {
            player->setDebt(fee);
            player->setDebtOwner(owner->getName());
        }
    }
}

