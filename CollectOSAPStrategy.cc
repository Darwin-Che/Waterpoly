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

    // Check the number of times the player passed OSAP
    int timesPassed = 0;
    if (moveAmount > 0) {
        for (int i = 0; i < moveAmount; ++i) {
            if (playerLocation == OSAPLocation)
                timesPassed++;
            playerLocation--;
            if (playerLocation == -1)
                playerLocation += totalSquares;
        }
    } else if (moveAmount < 0) {
        for (int i = 0; i < -moveAmount; ++i) {
            if (playerLocation == OSAPLocation)
                timesPassed++;
            playerLocation++;
            if (playerLocation == totalSquares)
                playerLocation = 0;
        }
    }
    
    for (int i = 0; i < timesPassed; ++i) {
        out << "You stepped on or over Collect OSAP square. "
            << "You will receive $200." << std::endl;
        // The work will be done by MoneyStrategy to avoid repeated code
        MoneyStrategy strat(200);
        strat.acceptVisitor(player, board, in, out);
    }
}

