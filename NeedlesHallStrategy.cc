#include "NeedlesHallStrategy.h"
#include "MoneyStrategy.h"
#include "Dice.h"
#include <cstdlib>

// Constructor: Set up the strategies based on the probabilities given in specification
NeedlesHallStrategy::NeedlesHallStrategy()
{
    strategies.push_back(std::make_shared<MoneyStrategy>(-200));
    strategies.push_back(std::make_shared<MoneyStrategy>(200));
    for (int i = 1; i <= 2; ++i)
    {
        strategies.push_back(std::make_shared<MoneyStrategy>(-100));
        strategies.push_back(std::make_shared<MoneyStrategy>(100));
    }
    for (int i = 1; i <= 3; ++i)
    {
        strategies.push_back(std::make_shared<MoneyStrategy>(-50));
        strategies.push_back(std::make_shared<MoneyStrategy>(50));
    }
    for (int i = 1; i <= 6; ++i)
    {
        strategies.push_back(std::make_shared<MoneyStrategy>(25));
    }
}

// The player will gain or lose a random amount of money
void NeedlesHallStrategy::acceptVisitor(std::shared_ptr<Player> player,
                                        std::shared_ptr<Board> board, std::istream &in, std::ostream &out)
{

    // There is 1% chance to receive a cup instead of the normal effects
    out << "You stepped on Needles Hall square. A random outcome will occur." << std::endl;
    if (Player::getTotalNumCups() < 4)
    {
        int cupans = Dice::cup();
        if (cupans == 0)
        {
            player->setNumCups(player->getNumCups() + 1);
            out << "Luckily, you received a Roll Up the Rim Cup." << std::endl;
            return;
        }
    }

    // Randomly pick a strategy from the vector and apply it to the player
    int randomResult = rand() % 18;
    strategies[randomResult]->acceptVisitor(player, board, in, out);
}
