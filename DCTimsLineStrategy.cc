#include "DCTimsLineStrategy.h"
#include "MoveStrategy.h"
#include <cstdlib>

DCTimsLineStrategy::DCTimsLineStrategy()
    : roll1(0), roll2(0) {}

int DCTimsLineStrategy::roll() {
    int result = rand() % 6;
    if (result == 0)
        result = 6;
    return result;
}

void DCTimsLineStrategy::rollDouble(std::shared_ptr<Player> player, std::ostream out) {
    roll1 = roll();
    roll2 = roll();
    out << "You rolled " << roll1 << " and " << roll2 << "." << std::endl;
    if (roll1 == roll2) {
        player->setIsJailed(false);
        player->setNumJailed(0);
        out << "You rolled double! You can leave DC Tims Line now." << std::endl;
    } else {
        player->setNumJailed(player->getNumJailed() + 1);
        out << "Sorry, you did not roll double. You are still stuck." << std::endl;
    }
}

void DCTimsLineStrategy::payFee(std::shared_ptr<Player> player, std::ostream ouyt) {
    if (player->getMoney() >= 50) {
        player->setMoney(player->getMoney() - 50);
    } else {
        player->setDebt(50);
        player->setDebtOwner("BANK");
    }
    player->setIsJailed(false);
    player->setNumJailed(0);
    out << "You paid $50 to leave the DC Tims Line. "
        << "Now you can move as usual." << std::endl;
}

void DCTimsLineStrategy::payCup(std::shared_ptr<Player> player, std::ostream out) {
    if (player->getNumCups() > 0) {
        player->setNumCups(player->getNumCups() - 1);
        player->setIsJailed(false);
        player->setNumJailed(0);
        out << "You spent a Roll Up the Rim Cup to leave DC Tims Line. "
            << "Now you can move as usual." << std::endl;
    } else {
        out << "Sorry. You do not have any Roll Up the Rim Cup left." << std::endl
            << "Please use a different command. Type either 1 or 2." << std::endl;
    }
}

void DCTimsLineStrategy::acceptVisitor(std::shared_ptr<Player> player,
        std::shared_ptr<Board> board, std::istream in, std::ostream out) {
    if (player->getIsJailed() == false)
        return;

    out << "You are stuck in DC Tims line, but you really want to leave." << std::endl
        << "Type 1 if you want to try to roll doubles, type 2 if you want to pay $50, "
        << "type 3 if you want to use a Roll Up the Rim Cup.";
    bool finished = false;

    while (finished == false) {
        std::string choice;
        while (choice != "1" && choice != "2" && choice != "3")
            in >> choice;
        if (choice == "1") {
            rollDouble(player, out);
            finished = true;
        } else if (choice == "2") {
            payFee(player, out);
            finished = true;
        } else {
            payCup(player, out);
            if (player->getIsJailed() == false)
                finished = true;
        }
    }

    if (player->numJailed() == 3) {
        out << "This is your third turn of being stuck in DC Tims Line, "
            << "so you must leave now." << std::endl;
        out << "Type 2 if you want to pay $50, type 3 if you want to "
            << "spend a Roll Up the Rim Cup." << std::endl;
        finished = false;

        while (finished == false) {
            std::string choice;
            while (choice != "2" && choice != "3")
                in >> choice;
            if (choice == "2") {
                payFee(player, out);
                finished = true;
            } else {
                payCup(player, out);
                if (player->getIsJailed() == false)
                    finished = true;
            }
        }
    }

    if (player->getIsJailed() == false) {
        if (roll1 == 0 && roll2 == 0) {
            roll1 = roll();
            roll2 = roll();
            out << "You rolled " << roll1 << " and " << roll2 << "." << std::endl;
        }
        MoveStrategy strat(roll1 + roll2);
        strat.acceptVisitor(player, board, in, out);
    }
}

