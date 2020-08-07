#include "GymStrategy.h"
#include "Building.h"
#include <cstdlib>

// The player will pay rent to the gym's owner if applicable
void GymStrategy::acceptVisitor(std::shared_ptr<Player> player,
        std::shared_ptr<Board> board, std::istream& in, std::ostream& out) {
    std::shared_ptr<Square> square = board->getSquare(player->getPosition());
    std::shared_ptr<Building> building = dynamic_pointer_cast<Building>(square);

    if (building != nullptr) {
        std::string buildingName = building->getName();
        std::shared_ptr<Player> owner = board->getOwner(buildingName);
        out << "You stepped on Gym " << buildingName << "." << std::endl;

        // No rent if the gym is mortgaged or if the owner landed on it
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

        // The rent will be proportional to the sum of two dice
        int roll1 = rand() % 6;
        if (roll1 == 0)
            roll1 = 6;
        int roll2 = rand() % 6;
        if (roll2 == 0)
            roll2 = 6;
        int numOwned = board->numNeighbourOwned(buildingName);
        int fee = 0;
        if (numOwned == 1)  // rent = 4 * sum of dice if owner controls one gym
            fee = 4 * (roll1 + roll2);
        else  // rent = 10 * sum of dice if owner controls two gyms
            fee = 10 * (roll1 + roll2);

        if (player->getMoney() >= fee) {
            player->setMoney(player->getMoney() - fee);
            owner->setMoney(owner->getMoney() + fee);
        } else {
            player->setDebt(fee);
            player->setDebtOwner(owner->getName());
        }
        out << "You paid $" << fee << " rent to the property's owner "
            << owner->getName() << "." << std::endl;
    }
}

