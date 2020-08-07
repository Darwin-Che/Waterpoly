#include "MoneyStrategy.h"

// Constructor
MoneyStrategy::MoneyStrategy(int gainAmount)
    gainAmount(gainAmount) {}

// Player will gain an amount of money equal to the private field gainAmount
void MoneyStrategy::acceptVisitor(std::shared_ptr<Player> player,
        std::shared_ptr<Board> board, std::istream& in, std::ostream& out) {
    int currentMoney = player->getMoney();
    if (currentMoney >= -gainAmount) {
        player->setMoney(currentMoney + gainAmount);
    } else {  // If player does not have enough money, he/she owes debt
        player->setDebt(-gainAmount);
        player->setDebtOwner("BANK");
    }

    if (gainAmount >= 0)
        out << "You gained $" << gainAmount << " dollars." << std::endl;
    else
        out << "You lost $" << -gainAmount << " dollars." << std::endl;
}

