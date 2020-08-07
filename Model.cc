#include "Model.h"
#include "View.h"
#include "Player.h"
#include "Square.h"
#include "Building.h"
#include "AcademicBuilding.h"
#include "Board.h"
#include "VisitStrategy.h"
#include <vector>
#include <map>
#include <string>
#include <vector>
#include <algorithm>

void Model::payDebt(std::shared_ptr<Player> p1)
{
    if (p1->getDebt() > 0 && p1->canPayDebt())
    {
        std::shared_ptr<Player> p2 = allPlayers[p1->getDebtOwner()];
        int amount = p1->getDebt();
        p1->setMoney(p1->getMoney() - amount);
        p2->setMoney(p2->getMoney() + amount);
        p1->setDebt(0);
        p1->setDebtOwner("");
    }
}

bool Model::squareTradable(std::shared_ptr<Square> s)
{
    Building* sBuilding = dynamic_cast<Building*>(s.get());
    AcademicBuilding* sAcademic = dynamic_cast<AcademicBuilding*>(s.get());

    bool checkProperty_type = (sBuilding != nullptr);
    if (!checkProperty_type) {
        show(s->getName() + " is not an Ownable Bulding!");
        return false;
    }

    bool checkProperty_Mort = (sBuilding->getIsMortgaged() == false);
    if (!checkProperty_Mort) {
        show(s->getName() + " must first be unmortgaged!");
        return false;
    }

    bool checkProperty_Impr = (sAcademic != nullptr) || (sAcademic->getImprovementLevel() == 0);
    if (!checkProperty_Impr) {
        show("All improvement of " + s->getName() + " must first be sold!");
        return false;
    }
    return true;
}

bool deductMoney(std::shared_ptr<Player> p, int price, const std::string& debtor)
{
    if (p->getMoney() >= price)
    {
        p->setMoney(p->getMoney() - price);
        return true;
    }
    else {
        if (p->getDebt() > 0) {
            if (p->getDebtOwner() != debtor) {
                std::cerr << "This should never happen: a player own to two debtor. ";
            }
        }
        p->setDebt(price + p->getDebt());
        p->setDebtOwner(debtor);
        return false;
    }
}

std::pair<std::string, int> Model::auctionHelper()
{
    std::string maxOfferer;
    int maxOffer = -1;
    int i = 0;

    while (true) {
        if (i == playerOrder.size())
        {
            if (maxOffer == -1) {
                show("No one makes an offer, all assests go to Bank!");
                break;
            }
            else {
                i = 0;
            }
        }
        std::string pyn = playerOrder[i];
        if (pyn == maxOfferer) {
            show("The bid ends with max offer of " + std::to_string(maxOffer) + " from " + maxOfferer);
            break;
        }
        show("Current bidding Player: " + pyn + " with highest offer: " + std::to_string(maxOffer) + " from " + maxOfferer);
        show("How much would you offer? ( input \"-1\" if you decide to skip ) ");
        int offer;
        while (true) {
            if (!(min >> offer)) {
                if (min.eof()) break;
                min.clear();
                min.ignore();
                show("Your command is invalid! Please put in an integer:");
            }
            else {
                if (allPlayers[pyn]->getDebt() > 0 || offer > allPlayers[pyn]->getMoney())
                {
                    show("You don't have that much money left! Please enter again: ( input \"-1\" if you decide to skip ) ");
                }
                else {
                    break;
                }
            }
        }
        if (offer > maxOffer) {
            show("You have offered a higher amount at: " + std::to_string(offer));
            maxOffer = offer;
            maxOfferer = pyn;
        }
        ++i;
    }

    return std::make_pair(maxOfferer, maxOffer);
}

void Model::getInfo(std::shared_ptr<Square> s)
{
    mout << s->getInfo() << std::endl;
    std::shared_ptr<Building> b = std::dynamic_pointer_cast<Building>(s);
    std::shared_ptr<AcademicBuilding> ab = std::dynamic_pointer_cast<AcademicBuilding>(s);
    if (ab != nullptr) {
        int fee = ab->getImprovementFee();
        if (board->inMonopoly(ab->getName()) && ab->getImprovementLevel() == 0)
            fee = fee * 2;
        mout << ab->getName() << "'s Owner - " << board->getOwner(ab->getName()) << std::endl;
        mout << "Current Rent - " << fee << std::endl;
        mout << "Monopoly block: ";
        // output all names of the block
        // output a list of numbers
    }
    else if (b != nullptr)
    {
        std::string fee;
        if (b->getName() == "PAC" || b->getName() == "CIF")
        {
            if (board->numNeighbourOwned(b->getName()) == 1) fee = "four times dice";
            if (board->numNeighbourOwned(b->getName()) == 2) fee = "ten times dice";
        }
        if (b->getName() == "MKV" || b->getName() == "UWP" || b->getName() == "V1" || b->getName() == "REV")
        {
            int numOwned = board->numNeighbourOwned(b->getName());
            if (numOwned == 1)
                fee = "25";
            else if (numOwned == 2)
                fee = "50";
            else if (numOwned == 3)
                fee = "100";
            else
                fee = "200";
        }
        mout << b->getName() << "'s Owner - " << board->getOwner(ab->getName()) << std::endl;
        mout << "Current Rent - " << fee << std::endl;
        mout << "Monopoly block: ";
        // output all names of the block
        // output a list of numbers
    }
    else
    {
    }
}

Model::Model(std::istream &tin, std::ostream &tout)
    : min{ tin }, mout{ tout }
{
}

void Model::playerProceed(const std::string &pn, int steps)
{
    std::shared_ptr<Player> p = allPlayers[pn];
    if (p->getIsJailed()) {
        strategies[board->getSquareLocation("DC Times Line")].acceptVisitor(p, board, min, mout);
    }
    else {
        p->setPosition((p->getPosition() + steps) % board->getTotalSquareNum());
        strategies[board->getSquareLocation("COLLECT OSAP")].acceptVisitor(p, board, min, mout);
        int prevPos = p->getPosition();
        strategies[p->getPosition()].acceptVisitor(p, board, min, mout);
        while (prevPos != p->getPosition() && !(p->getIsJailed())) {
            strategies[board->getSquareLocation("COLLECT OSAP")].acceptVisitor(p, board, min, mout);
            prevPos = p->getPosition();
            strategies[p->getPosition()].acceptVisitor(p, board, min, mout);
        }
    }
}

void Model::gotoTims(const std::string &pn)
{
    allPlayers[pn]->setIsJailed(true);
    allPlayers[pn]->setNumJailed(0);
    strategies[board->getSquareLocation("DC Times Line")].acceptVisitor(p, board, min, mout);
}

void Model::show(const std::string &message)
{
    mout << message << std::endl;
}

bool Model::canNext(const std::string &player)
{
    return allPlayers[player]->getDebt() == 0;
}

void Model::trade(const std::string &pn1, const std::string &pn2, const std::string &property, int price)
{
    std::shared_ptr<Square> s = board->getSquareBuilding(property);

    // checking process 
    bool checkP1 = (board->getOwner(property) == allPlayers[pn1]);
    if (!checkP1) {
        show(property + " is not owned by " + pn1);
        return;
    }
    bool checkP2_debt = (allPlayers[pn2]->getDebt() == 0);
    if (!checkP2_debt) {
        show(pn2 + " is in debt, cannot trade before pays off debt!");
        return;
    }
    bool checkP2_money = (allPlayers[pn2]->getMoney() >= price);
    if (!checkP2_money) {
        show(pn2 + " does not have enough money to this offer!");
        return;
    }

    if (!squareTradable(s)) return;

    // trade
    allPlayers[pn1]->setMoney(allPlayers[pn1]->getMoney() + price);
    allPlayers[pn2]->setMoney(allPlayers[pn2]->getMoney() - price);
    board->setOwner(property, allPlayers[pn2]);

    payDebt(allPlayers[pn1]);

}

void Model::trade(const std::string &pn1, const std::string &pn2, const std::string &property1, const std::string &property2)
{
    std::shared_ptr<Square> s1 = board->getSquareBuilding(property1);
    std::shared_ptr<Square> s2 = board->getSquareBuilding(property2);

    // checking process 
    bool checkP1_prop = (board->getOwner(property1) == allPlayers[pn1]);
    if (!checkP1_prop) {
        show(property1 + " is not owned by " + pn1);
        return;
    }

    bool checkP2_prop = (board->getOwner(property2) == allPlayers[pn2]);
    if (!checkP2_prop) {
        show(property2 + " is not owned by " + pn2);
        return;
    }

    bool checkP1_debt = (allPlayers[pn1]->getDebt() == 0);
    if (!checkP1_debt) {
        show(pn1 + " is in debt, cannot trade before pays off debt!");
        return;
    }

    bool checkP2_debt = (allPlayers[pn2]->getDebt() == 0);
    if (!checkP2_debt) {
        show(pn2 + " is in debt, cannot trade before pays off debt!");
        return;
    }

    if (!squareTradable(s1)) return;
    if (!squareTradable(s2)) return;

    // trade
    board->setOwner(property1, allPlayers[pn2]);
    board->setOwner(property2, allPlayers[pn1]);
}

void Model::improve(const std::string &pn, const std::string &property, bool action)
{
    std::shared_ptr<Square> s = board->getSquareBuilding(property);
    std::shared_ptr<AcademicBuilding> sAcademic = std::dynamic_pointer_cast<AcademicBuilding>(s);
    // check valid
    bool checkProperty_Impr = (sAcademic.get() != nullptr);
    // check building is improvable
    if (!checkProperty_Impr) {
        show(property + " is not an improvable Square!");
        return;
    }
    // check building is in monopoly
    if (!(board->inMonopoly(property))) {
        show(property + "'s Monopoly block is not entirely owned by you!");
        return;
    }
    // improve
    if (action)
    {
        // check if player is in debt
        if (allPlayers[pn]->getDebt() > 0) {
            show("You are still in Debt! First pay off your debt then improve the building!");
        }
        // check improvement is at maximum
        if (sAcademic->getImprovementLevel() > 4) {
            show(property + "'s improvement level is already at maximum!");
            return;
        }
        int cost = sAcademic->getImprovementCost();
        // check if player can afford
        if (allPlayers[pn]->getMoney() < cost) {
            show("Your money ( " + std::to_string(allPlayers[pn]->getMoney()) + " ) is insufficient for improvement cost ( " + std::to_string(cost) + " )!");
            return;
        }
        // execute change
        sAcademic->setImprovementLevel(sAcademic->getImprovementLevel() + 1);
        allPlayers[pn]->setMoney(allPlayers[pn]->getMoney() - cost);
    }
    else
    {
        // check improvement is at minimum
        if (sAcademic->getImprovementLevel() < 1) {
            show(property + "'s improvement level is already at minimum!");
            return;
        }
        int refund = sAcademic->getImprovementCost() / 2;
        // execute change
        sAcademic->setImprovementLevel(sAcademic->getImprovementLevel() - 1);
        allPlayers[pn]->setMoney(allPlayers[pn]->getMoney() + refund);
        // see if debt can be paid
        payDebt(allPlayers[pn]);
    }
}

void Model::mortgage(const std::string &pn, const std::string &property, bool action)
{
    std::shared_ptr<Square> s = board->getSquareBuilding(property);
    std::shared_ptr<Building> b = std::dynamic_pointer_cast<Building>(s);
    // check valid
    bool checkProperty = (b.get() != nullptr);
    // check building is improvable
    if (!checkProperty) {
        show(property + " is not an ownable property!");
        return;
    }
    // improve
    if (action)
    {
        // check status
        if (b->getIsMortgaged()) {
            show(property + "'s is already mortgaged!");
            return;
        }
        int refund = b->getPurchaseCost() * 0.5;
        // execute change
        b->setIsMortgaged(true);
        allPlayers[pn]->setMoney(allPlayers[pn]->getMoney() + refund);
        // see if debt can be paid
        payDebt(allPlayers[pn]);
    }
    else
    {
        // check if player is in debt
        if (allPlayers[pn]->getDebt() > 0) {
            show("You are still in Debt! First pay off your debt then unmortgage the building!");
        }
        // check improvement is at maximum
        if (!b->getIsMortgaged()) {
            show(property + "'s improvement level is already unmortgaged!");
            return;
        }
        int cost = b->getPurchaseCost() * 0.6;
        // check if player can afford
        if (allPlayers[pn]->getMoney() < cost) {
            show("Your money ( " + std::to_string(allPlayers[pn]->getMoney()) + " ) is insufficient for unmortgagement cost ( " + std::to_string(cost) + " )!");
            return;
        }
        // execute change
        b->setIsMortgaged(false);
        allPlayers[pn]->setMoney(allPlayers[pn]->getMoney() - cost);
    }
}

bool Model::bankrupt(const std::string &pn)
{
    if (allPlayers[pn]->getDebt() > 0)
    {
        show("You are able to bankrupt, preparing to Auction all your property!");
        auctionPlayer(pn);
        allPlayers[pn]->dropOut();
        allPlayers.erase(pn);
        playerOrder.erase(std::remove(playerOrder.begin(), playerOrder.end(), pn), playerOrder.end());
        return true;
    }
    else
    {
        show("You are not in debt, cannot bankrupt!");
        return false;
    }
}

void Model::auctionPlayer(const std::string &pn)
{
    show("Auction Start! Bid for Player " + pn);
    show("These are all assets under auction: ");
    std::shared_ptr<Player> p = allPlayers[pn];
    getInfo(pn);

    std::pair<std::string, int> result = auctionHelper();
    std::string maxOfferer = result.first;
    int maxOffer = result.second;

    if (maxOffer >= 0) {
        // for all property
        for (auto & s : board->getAssets(pn))
        {
            std::shared_ptr<Building> b = std::dynamic_pointer_cast<Building>(s);
            if (b.get() == nullptr) std::cerr << "This should never happen: a player owns a nonpropery";
            std::shared_ptr<AcademicBuilding> ab = std::dynamic_pointer_cast<AcademicBuilding>(s);

            if (b->getIsMortgaged()) {
                show("Player " + maxOfferer + " would you like to unmortgage this property: " + b->getName() + "? ( yes/no )");
                std::string ans;
                while (true) {
                    if (!(min >> ans)) {
                        if (min.eof()) break;
                        min.clear();
                        min.ignore();
                        show("Your command is invalid! Please put in \"yes\" or \"no\": ");
                    }
                    else {
                        if (ans == "yes")
                            deductMoney(allPlayers[maxOfferer], b->getPurchaseCost() * 0.6, "BANK");
                        else if (ans == "no")
                            deductMoney(allPlayers[maxOfferer], b->getPurchaseCost() * 0.1, "BANK");
                        else
                            show("Your command is invalid! Please put in \"yes\" or \"no\": ");
                    }
                }
            }
            if (ab.get() != nullptr) {
                ab->setImprovementLevel(0);
            }
            board->setOwner(b->getName(), allPlayers[maxOfferer]);
        }
    }
    else {
        // for all property, give to bank
        for (auto & s : board->getAssets(pn))
        {
            std::shared_ptr<Building> b = std::dynamic_pointer_cast<Building>(s);
            if (b.get() == nullptr) std::cerr << "This should never happen: a player owns a nonpropery";
            std::shared_ptr<AcademicBuilding> ab = std::dynamic_pointer_cast<AcademicBuilding>(s);

            if (ab.get() != nullptr) {
                ab->setImprovementLevel(0);
            }
            b->setIsMortgaged(false);
            std::shared_ptr<Player> bank;
            board->setOwner(b->getName(), bank);
        }
    }
}

void Model::auctionBuilding(const std::string &bn)
{
    std::shared_ptr<Building> b = std::dynamic_pointer_cast<Building>(board->getSquareBuilding(bn));
    // check valid
    bool checkProperty = (b.get() != nullptr);
    // check building is improvable
    if (!checkProperty) {
        show(bn + " is not an ownable property!");
        return;
    }

    // start
    show("Auction Start! Bid for Building " + bn);
    getInfo(b);

    std::pair<std::string, int> result = auctionHelper();
    std::string maxOfferer = result.first;
    int maxOffer = result.second;

    if (maxOffer >= 0) {
        // this one will deduct money, player is guarenteed to have enough money, for is checked at auction stage
        deductMoney(allPlayers[maxOfferer], b->getPurchaseCost(), "BANK");
        // this one should never be called, but remains here if to expand the method's functionality in the future
        if (b->getIsMortgaged()) {
            show("Player " + maxOfferer + " would you like to unmortgage this property: " + b->getName() + "? ( yes/no )");
            std::string ans;
            while (true) {
                if (!(min >> ans)) {
                    if (min.eof()) break;
                    min.clear();
                    min.ignore();
                    show("Your command is invalid! Please put in \"yes\" or \"no\": ");
                }
                else {
                    if (ans == "yes")
                        deductMoney(allPlayers[maxOfferer], b->getPurchaseCost() * 0.6, "BANK");
                    else if (ans == "no")
                        deductMoney(allPlayers[maxOfferer], b->getPurchaseCost() * 0.1, "BANK");
                    else
                        show("Your command is invalid! Please put in \"yes\" or \"no\": ");
                }
            }
        }
        // another safty measure
        std::shared_ptr<AcademicBuilding> ab = std::dynamic_pointer_cast<AcademicBuilding>(board->getSquareBuilding(bn));
        if (ab.get() != nullptr) {
            ab->setImprovementLevel(0);
        }
        board->setOwner(b->getName(), allPlayers[maxOfferer]);

    }
    else {
        b->setIsMortgaged(false);
        std::shared_ptr<AcademicBuilding> ab = std::dynamic_pointer_cast<AcademicBuilding>(board->getSquareBuilding(bn));
        if (ab.get() != nullptr) {
            ab->setImprovementLevel(0);
        }
        std::shared_ptr<Player> bank;
        board->setOwner(b->getName(), bank);
    }
}

void Model::getInfo()
{
    for (auto & p : playerOrder)
    {
        getInfo(p);
    }
}

void Model::getInfo(const std::string &pn)
{
    std::shared_ptr<Player> p = allPlayers[pn];
    std::shared_ptr<Square> s = board->getSquare(p->getPosition());
    mout << "Player " << pn << " (" << p->getSymbol() << ") "<< " is at " << s->getName()
        << " Money - " << p->getMoney()
        << " Debt - " << p->getDebt() << (p->getDebt() > 0 ? " toward " + p->getDebtOwner() : "")
        << " Jailed - " << std::boolalpha << p->getIsJailed() << (p->getIsJailed() ? " for " + p->getNumJailed() + " turns" : "")
        << " Cups - " << p->getNumCups() << std::endl;
    mout << "All Assets: " << std::endl;
    for (auto & s: board->getAssets(pn))
    {
        mout << "\t";
        getInfo(s);
    }
}

void Model::save(std::ostream &out)
{
    out << playerOrder.size() << std::endl;
    for (auto & pn : playerOrder) {
        std::shared_ptr<Player> p = allPlayers[pn];
        out << p->getName() << " " << p->getSymbol() << " " << p->getNumCups() << " " << p->getMoney() << " " << p->getPosition();
        if (p->getPosition() == board->getSquareLocation("DC Tims Line"))
        {
            out << p->getIsJailed();
            if (p->getIsJailed()) out << p->getNumJailed();
        }
        out << std::endl;
        for (auto & s : board->getAllSquare())
        {
            out << s->getInfo();
        }
    }
}
