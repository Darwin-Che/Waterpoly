#include "SLCStrategy.h"
#include "MoveStrategy.h"
#include <cstdlib>

SLCStrategy::SLCStrategy() {
    for (int i = 1; i <= 3; ++i) {
        strategies.push_back(std::make_shared<MoveStrategy>(-3));
        strategies.push_back(std::make_shared<MoveStrategy>(1));
    }
    for (int i = 1; i <= 4; ++i) {
        strategies.push_back(std::make_shared<MoveStrategy>(-2));
        strategies.push_back(std::make_shared<MoveStrategy>(-1));
        strategies.push_back(std::make_shared<MoveStrategy>(2));
        strategies.push_back(std::make_shared<MoveStrategy>(3));
    }
}

void SLCStrategy::acceptVisitor(std::shared_ptr<Player> player,
            std::shared_ptr<Board> board, std::istream in, std::ostream out) {
    out << "You stepped on SLC square. A random outcome will occur." << std::endl;
    if (Player::totalNumCups < 4 && rand() % 100 == 0) {
        player->setNumCups(player->getNumCups() + 1);
        out << "Luckily, you received a Roll Up the Rim Cup." << std::endl;
        return;
    }

    int timsLocation = board->getSquare("DC Tims Line")->getLocation();
    int moveAmount = timsLocation - player->getPosition();
    strategies.push_back(std::make_shared<MoveStrategy>(moveAmount));

    int OSAPLocation = board->getSquare("Collect OSAP")->getLocation();
    moveAmount = OSAPLocation - player->getPosition();
    strategies.push_back(std::make_shared<MoveStrategy>(moveAmount));

    int randomResult = rand() % 24;
    strategies[randomResult]->acceptVisitor(player, board, in, out);
    strategies.pop_back();
    strategies.pop_back();
}

