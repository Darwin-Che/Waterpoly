#include "GoToTimsStrategy.h"
#include "MoveStrategy.h"

// The player is directly sent to DC Tims Line
void GoToTimsStrategy::acceptVisitor(std::shared_ptr<Player> player,
        std::shared_ptr<Board> board, std::istream& in, std::ostream& out) {
    out << "You stepped on Go To Tims square. You will be moved to DC Tims Line." << std::endl;
    int timsLocation = board->getSquareBuilding("DC Tims Line")->getLocation();

    // The work will be done by MoveStrategy to avoid duplicate code
    MoveStrategy strat(timsLocation - player->getPosition());
    strat.acceptVisitor(player, board, in, out);
}

