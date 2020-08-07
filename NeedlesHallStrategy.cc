#include "NeedlesHallStrategy.h"
#include "MoneyStrategy.h"
#include <cstdlib>

NeedlesHallStrategy::NeedlesHallStrategy() {
    strategies.push_back(std::make_shared<MoneyStrategy>(-200));
    strategies.push_back(std::make_shared<MoneyStrategy>(200));
    for (int i = 1; i <= 2; ++i) {
        strategies.push_back(std::make_shared<MoneyStrategy>(-100));
        strategies.push_back(std::make_shared<MoneyStrategy>(100));
    }
    for (int i = 1; i <= 3; ++i) {
        strategies.push_back(std::make_shared<MoneyStrategy>(-50));
        strategies.push_back(std::make_shared<MoneyStrategy>(50));
    }
    for (int i = 1; i <= 6; ++i) {
        strategies.push_back(std::make_shared<MoneyStrategy>(25));
    }
}

void NeedlesHallStrategy::acceptVisitor(std::shared_ptr<Player> player,
            std::shared_ptr<Board> board, std::istream in, std::ostream out) {
    out << "You stepped on Needles Hall square. A random outcome will occur." << std::endl;
    if (Player::totalNumCups < 4 && rand() % 100 == 0) {
        player->setNumCups(player->getNumCups() + 1);
        out << "Luckily, you received a Roll Up the Rim Cup." << std::endl;
        return;
    }

    int randomResult = rand() % 18;
    strategies[randomResult]->acceptVisitor(player, board, in, out);
}

