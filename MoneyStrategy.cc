#include "MoneyStrategy.h"

MoneyStrategy::MoneyStrategy(int gainAmount)
    gainAmount(gainAmount) {}

void MoneyStrategy::acceptVisitor(std::shared_ptr<Player> player,
        std::shared_ptr<Board> board, std::istream in, std::ostream out) {
    int currentMoney = player->getMoney();
    if (currentMoney >= -gainAmount) {
        player->setMoney(currentMoney + gainAmount);
    } else {
        player->setDebt(-gainAmount);
        player->setDebtOwner("BANK");
    }

    if (gainAmount >= 0)
        out << "You gained $" << gainAmount << " dollars." << std::endl;
    else
        out << "You lost $" << -gainAmount << " dollars." << std::endl;
}

