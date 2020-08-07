#include "MoveStrategy.h"

MoveStrategy::MoveStrategy(int moveAmount)
    moveAmount(moveAmount) {}

void MoveStrategy::acceptVisitor(std::shared_ptr<Player> player,
        std::shared_ptr<Board> board, std::istream in, std::ostream out) {
    int newPosition = player->getPosition() + moveAmount;
    newPosition = newPosition % board->getTotalSquareNum();
    player->setPosition(newPosition);
    std::string squareName = board->getSquare(newPosition)->getName();

    if (moveAmount >= 0)
        out << "You moved forward " << moveAmount << " spots. ";
    else
        out << "You moved backward " << moveAmount << " spots. ";
    out << "You arrived at " << squareName << " square." << std::endl;

    if (squareName == "DC Tims Line") {
        player->setIsJailed(true);
        player->setNumJailed(0);
    }
}


