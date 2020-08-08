#include "TuitionStrategy.h"
#include "MoneyStrategy.h"

// The player will pay $300 tuition or 10% of his/her total networth
void TuitionStrategy::acceptVisitor(std::shared_ptr<Player> player,
        std::shared_ptr<Board> board, std::istream& in, std::ostream& out) {
    out << "You stepped on the Tuition square. You must pay tuition." << std::endl
        << "Type 1 if you want to pay $300 tuition, type 2 if you want to "
        << "pay 10\% of your total networth." << std::endl
        << "You cannot see your networth before making a choice." << std::endl;
    std::string choice;
    while (choice != "1" && choice != "2") {
        in >> choice;
    }

    // Calculate the amount of tuition according to player's choice
    int payment = 0;
    if (choice == "1") {
        payment = 300;
    } else {
        int networth = board->calcNetworth(player->getName());
        payment = static_cast<int>(0.1 * networth);
    }

    // The work will be done by MoneyStrategy to avoid duplicate code
    MoneyStrategy strat(-payment);
    strat.acceptVisitor(player, board, in, out);
}

