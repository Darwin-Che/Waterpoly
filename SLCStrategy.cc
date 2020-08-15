#include "SLCStrategy.h"
#include "MoveStrategy.h"
#include "Dice.h"
#include <cstdlib>

// Constructor: Set up the strategies based on the probabilities given in doc
SLCStrategy::SLCStrategy()
{
    for (int i = 1; i <= 3; ++i)
    {
        strategies.push_back(std::make_shared<MoveStrategy>(-3)); // 0 2 4
        strategies.push_back(std::make_shared<MoveStrategy>(1));  // 1 3 5
    }
    for (int i = 1; i <= 4; ++i)
    {
        strategies.push_back(std::make_shared<MoveStrategy>(-2)); // 6 10 14 18
        strategies.push_back(std::make_shared<MoveStrategy>(-1)); // 7 11 15 19
        strategies.push_back(std::make_shared<MoveStrategy>(2));  // 8 12 16 20
        strategies.push_back(std::make_shared<MoveStrategy>(3));  // 9 13 17 21
    }
}

// The player will move forward or backward a random amount
void SLCStrategy::acceptVisitor(std::shared_ptr<Player> player,
                                std::shared_ptr<Board> board, std::istream &in, std::ostream &out)
{

    // There is 1% chance to receive a cup instead of normal effects
    out << "You stepped on SLC square. A random outcome will occur." << std::endl;
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

    int timsLocation = board->getSquareBuilding("DC Tims Line")->getLocation();
    int moveAmount = timsLocation - player->getPosition();
    strategies.push_back(std::make_shared<MoveStrategy>(moveAmount)); // 22

    int OSAPLocation = board->getSquareBuilding("COLLECT OSAP")->getLocation();
    moveAmount = OSAPLocation - player->getPosition();
    strategies.push_back(std::make_shared<MoveStrategy>(moveAmount)); // 23

    // Randomly pick a strategy from the vector and apply it to the player
    int randomResult = rand() % 24;

    if (Dice::testing_mode)
    {
        out << "In testing Mode, please choose the following: " << std::endl;
        out << "1: Back 3; \t 2: Back 2; \t 3: Back 1; \n";
        out << "4: Forward 1; \t 5: Forward 2; \t 6: Forward 3; \n";
        out << "7: Go to DC Tims Line \n";
        out << "8: Advance to Collect OSAP" << std::endl;
        int ans = 0;
        in >> ans;
        switch (ans)
        {
        case 1:
            randomResult = 0;
            break;
        case 2:
            randomResult = 6;
            break;
        case 3:
            randomResult = 7;
            break;
        case 4:
            randomResult = 1;
            break;
        case 5:
            randomResult = 8;
            break;
        case 6:
            randomResult = 9;
            break;
        case 7:
            randomResult = 22;
            break;
        case 8:
            randomResult = 23;
            break;
        default:
            break;
        }
    }

    strategies[randomResult]->acceptVisitor(player, board, in, out);
    if (randomResult == 22) // Go To DC Tims Line is selected
        player->setIsJailed(true);
    strategies.pop_back();
    strategies.pop_back();
}
