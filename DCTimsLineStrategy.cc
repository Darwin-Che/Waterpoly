#include "DCTimsLineStrategy.h"
#include "MoveStrategy.h"
#include <cstdlib>

// Constructor
DCTimsLineStrategy::DCTimsLineStrategy()
    : roll1(-1), roll2(-1) {}

// Utility method: roll a dice
int DCTimsLineStrategy::roll() {
    int result = rand() % 6;
    if (result == 0)
        result = 6;
    return result;
}

// Helper method: The player tries to roll doubles to leave Tims Line
void DCTimsLineStrategy::rollDouble(std::shared_ptr<Player> player, std::ostream& out) {
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

// Helper method: The player pays $50 fee to leave Tims Line
void DCTimsLineStrategy::payFee(std::shared_ptr<Player> player, std::ostream& out) {
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

// Helper method: The player tries to spend Roll Up the Rim Cup to leave Tims Line
void DCTimsLineStrategy::payCup(std::shared_ptr<Player> player, std::ostream& out) {
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

// The player attempts to leave Tims Line using one of three options
void DCTimsLineStrategy::acceptVisitor(std::shared_ptr<Player> player,
        std::shared_ptr<Board> board, std::istream& in, std::ostream& out) {
    // Nothing happens if the player directly landed on Tims Line
    if (player->getIsJailed() == false) {
        out << "You directly landed on DC Tims line, so nothing happened." << std::endl;
        return;
    }

    // Otherwise, ask the player how he/she plans to leave Tims Line
    out << "You are stuck in DC Tims line, but you really want to leave." << std::endl
        << "Type 1 if you want to try to roll doubles, type 2 if you want to pay $50, "
        << "type 3 if you want to use a Roll Up the Rim Cup.";
    bool finished = false;

    // Carry out the appropriate action based on the player's choice
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

    // If the player has already rolled 3 times including this time,
    // then the player must leave using either option 2 or option 3
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

    // If the player is no longer jailed, then the player can move
    // the same amount as the sum of the dice from his/her last roll
    if (player->getIsJailed() == false) {
        if (roll1 == -1 && roll2 == -1) {
            roll1 = roll();
            roll2 = roll();
            out << "You rolled " << roll1 << " and " << roll2 << "." << std::endl;
        }
        MoveStrategy strat(roll1 + roll2);
        strat.acceptVisitor(player, board, in, out);
    }
}

