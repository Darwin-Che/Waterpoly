#include "MoveStrategy.h"

// Constructor
MoveStrategy::MoveStrategy(int moveAmount)
    : moveAmount(moveAmount) {}

// Player will move an amount equal to the private field moveAmount
void MoveStrategy::acceptVisitor(std::shared_ptr<Player> player,
        std::shared_ptr<Board> board, std::istream& in, std::ostream& out) {
    player->moveForward(moveAmount, board->getTotalSquareNum()); 
    std::string squareName = board->getSquare(player->getPosition())->getName();

    if (moveAmount >= 0)
        out << "You moved forward " << moveAmount << " spots. ";
    else
        out << "You moved backward " << moveAmount << " spots. ";
    out << "You arrived at " << squareName << " square." << std::endl;

    // If the player landed on Tims Line, he/she is jailed
    if (squareName == "DC Tims Line") {
        player->setIsJailed(true);
        player->setNumJailed(0);
    }
}


