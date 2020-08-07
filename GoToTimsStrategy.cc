#include "GoToTimsStrategy.h"
#include "MoveStrategy.h"

void GoToTimsStrategy::acceptVisitor(std::shared_ptr<Player> player,
        std::shared_ptr<Board> board, std::istream in, std::ostream out) {
    out << "You stepped on Go To Tims square. You will be moved to DC Tims Line." << std::endl;
    int timsLocation = board->getSquare("DC Tims Line")->getLocation();
    MoveStrategy strat(timsLocation - player->getPosition());
    strat.acceptVisitor(player, board, in, out);
}

