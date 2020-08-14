#include "CollectOSAPStrategy.h"
#include "MoneyStrategy.h"

// The player will collect $200 OSAP fund
void CollectOSAPStrategy::acceptVisitor(std::shared_ptr<Player> player,
        std::shared_ptr<Board> board, std::istream& in, std::ostream& out) {
    if (player->getIsJailed() == true) return;

    // Check if the player landed on or passed over OSAP square 
    int OSAPLocation = board->getSquareBuilding("COLLECT OSAP")->getLocation();
    int playerLocation = player->getPosition();
    int moveAmount = player->getLastMoveAmount();
    int totalSquares = board->getTotalSquareNum();
    if (moveAmount == 0) return;
    
    if (moveAmount < totalSquares && playerLocation != OSAPLocation) {
        if (moveAmount > 0) {
            if (playerLocation < OSAPLocation) {
                if (moveAmount < totalSquares - OSAPLocation + 1 + playerLocation) return;
            } else if (playerLocation > OSAPLocation) {
                if (moveAmount < playerLocation - OSAPLocation + 1) return;
            }
        } else if (moveAmount < 0) {
            if (playerLocation < OSAPLocation) {
                if (moveAmount < OSAPLocation + 1 - playerLocation) return;
            } else if (playerLocation > OSAPLocation) {
                if (moveAmount < totalSquares - playerLocation + OSAPLocation + 1) return;
            }
        }
    }
 
    out << "You stepped on or over Collect OSAP square. "
        << "You will receive $200." << std::endl;

    // The work will be done by MoneyStrategy to avoid repeated code
    MoneyStrategy strat(200);
    strat.acceptVisitor(player, board, in, out);
}

