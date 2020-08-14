#include "ModelImplementation.h"
#include "ModelExcept.h"
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

bool ModelImplementation::checkPlayerDebt(const std::string &pn)
{
    if (allPlayers[pn]->getDebt() > 0)
        show(allPlayers[pn]->getName() + " is in Debt!");
    return allPlayers[pn]->getDebt() <= 0;
}

bool ModelImplementation::checkPlayerAfford(std::shared_ptr<Player> p, int price)
{
    if (p->getMoney() < price)
        show(p->getName() + " cannot afford the price!");
    return p->getMoney() >= price;
}

bool ModelImplementation::checkOwner(std::shared_ptr<Player> p, const std::string &bn)
{
    if (board->getOwner(bn) != p)
        show(bn + " is not owned by " + p->getName());
    return board->getOwner(bn) == p;
}

bool ModelImplementation::playerJailed(const std::string &pn)
{
    return allPlayers[pn]->getIsJailed();
}

bool ModelImplementation::existPlayer(const std::string &pn)
{
    if (allPlayers[pn].get() != nullptr)
        return true;
    show(pn + " is not a playername!");
    return false;
}

bool ModelImplementation::existBuilding(const std::string &bn)
{
    if (board->getSquareBuilding(bn).get() != nullptr)
        return true;
    show(bn + " is not a buildingname!");
    return false;
}

void ModelImplementation::loadPlayer(std::vector<std::shared_ptr<Player>> playerList)
{
    for (auto &p : playerList)
    {
        playerOrder.emplace_back(p->getName());
        allPlayers[p->getName()] = p;
    }
}

void ModelImplementation::clearPlayer()
{
    playerOrder.clear();
    allPlayers.clear();
}

void ModelImplementation::loadMap(std::shared_ptr<Board> tboard, std::vector<std::shared_ptr<VisitStrategy>> tstrategies)
{
    board = tboard;
    strategies = tstrategies;
}

void ModelImplementation::clearMap()
{
    board = std::shared_ptr<Board>();
    strategies.clear();
}

void ModelImplementation::payDebt(std::shared_ptr<Player> p1)
{
    if (p1->getDebt() > 0 && p1->canPayDebt())
    {
        int amount = p1->getDebt();
        p1->setMoney(p1->getMoney() - amount);
        if (p1->getDebtOwner() != "BANK")
        {
            std::shared_ptr<Player> p2 = allPlayers[p1->getDebtOwner()];
            p2->setMoney(p2->getMoney() + amount);
        }
        p1->setDebt(0);
        p1->setDebtOwner("");
    }
}

bool ModelImplementation::deductMoney(std::shared_ptr<Player> p, int price, const std::string &debtor)
{
    if (p->getMoney() >= price)
    {
        p->setMoney(p->getMoney() - price);
        return true;
    }
    else
    {
        if (p->getDebt() > 0)
        {
            if (p->getDebtOwner() != debtor)
            {
                std::cerr << "This should never happen: a player own to two debtor. ";
            }
        }
        p->setDebt(price + p->getDebt());
        p->setDebtOwner(debtor);
        return false;
    }
}

void ModelImplementation::getInfo(std::shared_ptr<Square> s)
{
    // print basic information
    mout << s->getInfo() << std::endl;
    // print specific information
    std::shared_ptr<Building> b = std::dynamic_pointer_cast<Building>(s);
    std::shared_ptr<AcademicBuilding> ab = std::dynamic_pointer_cast<AcademicBuilding>(s);
    if (ab != nullptr)
    {
        int fee = ab->getImprovementFee();
        if (board->inMonopoly(ab->getName()) && ab->getImprovementLevel() == 0)
            fee = fee * 2;
        std::shared_ptr<Player> p = board->getOwner(ab->getName());
        mout << ab->getName() << "'s Owner - " << (p.get() == nullptr ? "BANK" : p->getName()) << std::endl;
        mout << "Current Rent - " << fee << std::endl;
        // mout << "Monopoly block: ";
        // output all names of the block
        // output a list of numbers
    }
    else if (b != nullptr)
    {
        std::string fee;
        if (b->getName() == "PAC" || b->getName() == "CIF")
        {
            if (board->numNeighbourOwned(b->getName()) == 0)
                fee = "Zero (currently owned by Bank)";
            if (board->numNeighbourOwned(b->getName()) == 1)
                fee = "four times dice";
            if (board->numNeighbourOwned(b->getName()) == 2)
                fee = "ten times dice";
        }
        if (b->getName() == "MKV" || b->getName() == "UWP" || b->getName() == "V1" || b->getName() == "REV")
        {
            int numOwned = board->numNeighbourOwned(b->getName());
            if (numOwned == 0)
                fee = "Zero (currently owned by Bank)";
            if (numOwned == 1)
                fee = "25";
            else if (numOwned == 2)
                fee = "50";
            else if (numOwned == 3)
                fee = "100";
            else
                fee = "200";
        }
        std::shared_ptr<Player> p = board->getOwner(b->getName());
        mout << b->getName() << "'s Owner - " << (p.get() == nullptr ? "BANK" : p->getName()) << std::endl;
        mout << "Current Rent - " << fee << std::endl;
        // mout << "Monopoly block: ";
        // output all names of the block
        // output a list of numbers
    }
}

ModelImplementation::ModelImplementation(std::istream &tin, std::ostream &tout)
    : min{tin}, mout{tout}
{
}

void ModelImplementation::show(const std::string &message)
{
    mout << message << std::endl;
}

std::string ModelImplementation::nextPlayerName(const std::string &pn, bool force)
{
    if (!existPlayer(pn))
        return pn;

    if (allPlayers[pn]->getDebt() == 0 || force)
    {
        std::vector<std::string>::iterator it = std::find(playerOrder.begin(), playerOrder.end(), pn);
        if (it != playerOrder.end())
        {
            int index = std::distance(playerOrder.begin(), it);
            index++;
            if (index == playerOrder.size())
                index = 0;
            return playerOrder[index];
        }
        else
        {
            std::cerr << pn << " is not a player!" << std::endl;
            return pn;
        }
    }
    else
    {
        show("You are in debt, must first pay off your debt then next! If you cannot pay off, declare bankruptcy!");
        return pn;
    }
}

void ModelImplementation::getInfo()
{
    for (auto &p : playerOrder)
        this->getInfo(p);
}

void ModelImplementation::getInfo(const std::string &pn)
{
    if (!existPlayer(pn))
        return;
    std::shared_ptr<Player> p = allPlayers[pn];
    std::shared_ptr<Square> s = board->getSquare(p->getPosition());
    mout << "Player " << pn << " (" << p->getSymbol() << ") "
         << " is at " << s->getName()
         << " Money - " << p->getMoney()
         << " Debt - " << p->getDebt() << (p->getDebt() > 0 ? " toward " + p->getDebtOwner() : "")
         << " Jailed - " << std::boolalpha << p->getIsJailed() << (p->getIsJailed() ? (" for " + std::to_string(p->getNumJailed()) + " turns") : "")
         << " Cups - " << p->getNumCups() << std::endl;
    auto all = board->getAssets(pn);
    if (all.size() != 0)
        mout << "All Assets: " << std::endl;
    for (auto &s : board->getAssets(pn))
    {
        mout << "\t";
        mout << s->getInfo() << std::endl;
    }
}

void ModelImplementation::save(std::ostream &out, std::string pn)
{
    if (allPlayers[pn]->getDebt() > 0)
    {
        show("You must pay off debt before save the game!");
        return;
    }
    out << playerOrder.size() << std::endl;
    std::vector<std::string>::iterator i = std::find(playerOrder.begin(), playerOrder.end(), pn);
    while (true)
    {
        std::shared_ptr<Player> p = allPlayers[*i];
        out << p->getName() << " " << p->getSymbol() << " " << p->getNumCups() << " " << p->getMoney() << " " << p->getPosition();
        if (p->getPosition() == board->getSquareLocation("DC Tims Line"))
        {
            out << p->getIsJailed();
            if (p->getIsJailed())
                out << p->getNumJailed();
        }
        out << std::endl;
        i++;
        if (i == playerOrder.end())
            i = playerOrder.begin();
        if (*i == pn)
            break;
    }
    out << board->saveInfo();
}
