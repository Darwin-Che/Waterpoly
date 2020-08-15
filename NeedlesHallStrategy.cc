#include "NeedlesHallStrategy.h"
#include "MoneyStrategy.h"
#include "Dice.h"
#include <cstdlib>

// Constructor: Set up the strategies based on the probabilities given in specification
NeedlesHallStrategy::NeedlesHallStrategy()
{
    strategies.push_back(std::make_shared<MoneyStrategy>(-200)); // 0
    strategies.push_back(std::make_shared<MoneyStrategy>(200));  // 1
    for (int i = 1; i <= 2; ++i)
    {
        strategies.push_back(std::make_shared<MoneyStrategy>(-100)); // 2 4
        strategies.push_back(std::make_shared<MoneyStrategy>(100));  // 3 5
    }
    for (int i = 1; i <= 3; ++i)
    {
        strategies.push_back(std::make_shared<MoneyStrategy>(-50)); // 6 8 10
        strategies.push_back(std::make_shared<MoneyStrategy>(50));  // 7 9 11
    }
    for (int i = 1; i <= 6; ++i)
    {
        strategies.push_back(std::make_shared<MoneyStrategy>(25)); // 12 - 17
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

    if (Dice::testing_mode)
    {
        out << "In testing mode: enter the desired result:" << std::endl;
        out << "1: -200; \t 2: -100; \t 3: -50; \t 4: +25; \n";
        out << "5: +50; \t 6: +100; \t 7: +200" << std::endl;
        int ans = 0;
        in >> ans;
        switch (ans)
        {
        case 1:
            randomResult = 0;
            break;
        case 2:
            randomResult = 2;
            break;
        case 3:
            randomResult = 6;
            break;
        case 4:
            randomResult = 12;
            break;
        case 5:
            randomResult = 7;
            break;
        case 6:
            randomResult = 3;
            break;
        case 7:
            randomResult = 1;
            break;
        default:
            break;
        }
    }

    strategies[randomResult]->acceptVisitor(player, board, in, out);
}
