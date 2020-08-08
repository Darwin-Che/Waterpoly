#include "CollectOSAPStrategy.h"
#include "MoneyStrategy.h"

// The player will collect $200 OSAP fund
void CollectOSAPStrategy::acceptVisitor(std::shared_ptr<Player> player,
        std::shared_ptr<Board> board, std::istream& in, std::ostream& out) {
    if (player->getIsJailed() == true) return;

    // Check if the player landed on or passed over OSAP square
    int OSAPLocation = board->getSquareBuilding("COLLECT OSAP")->getLocation();
    int newLocation = player->getPosition();
    int oldLocation = player->getOldPosition();
    int forwardDistance = newLocation - oldLocation;
    if (forwardDistance < 0) forwardDistance += board->getTotalSquareNum();
    int backwardDistance = oldLocation - newLocation;
    if (backwardDistance < 0) backwardDistance += board->getTotalSquareNum();

    if (forwardDistance <= backwardDistance) { // The player moved forward
        if (newLocation >= oldLocation) {
            if (OSAPLocation <= oldLocation || OSAPLocation > newLocation)
                return;
        } else {
            if (!(OSAPLocation > oldLocation || OSAPLocation <= newLocation))
                return;
        }
    } else { // The player moved backward
        if (newLocation <= oldLocation) {
            if (OSAPLocation < newLocation || OSAPLocation >= oldLocation)
                return;
        } else {
            if (!(OSAPLocation < oldLocation || OSAPLocation >= newLocation))
                return;
        }
    }

    out << "You stepped on or over Collect OSAP square. "
        << "You will receive $200." << std::endl;

    // The work will be done by MoneyStrategy to avoid repeated code
    MoneyStrategy strat(200);
    strat.acceptVisitor(player, board, in, out);
}

