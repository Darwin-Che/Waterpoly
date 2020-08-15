#include "ModelImplPrimitive.h"
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

void ModelImplPrimitive::applyStrategy(std::shared_ptr<Player> p)
{
    strategies[p->getPosition()]->acceptVisitor(p, board, min, mout);
}

void ModelImplPrimitive::playerPayVisit(std::shared_ptr<Player> p)
{
    if (p->getPosition() != board->getSquareLocation("COLLECT OSAP"))
        strategies[board->getSquareLocation("COLLECT OSAP")]->acceptVisitor(p, board, min, mout);
    // if this is a building need auction
    if (board->getOwner(board->getSquare(p->getPosition())->getName()).get() == nullptr && std::dynamic_pointer_cast<Building>(board->getSquare(p->getPosition())).get() != nullptr)
        sellBuilding(p->getName(), board->getSquare(p->getPosition())->getName());
    else
        this->applyStrategy(p);
}

std::pair<std::string, int> ModelImplPrimitive::auctionHelper()
{
    std::string maxOfferer;
    int maxOffer = -1;
    int i = 0;
    std::vector<std::string> copyPlayerOrder{playerOrder};

    while (true)
    {
        // if i is out of bound
        if (i == copyPlayerOrder.size())
        {
            // if no one yet makes an offer, asset go to bank
            if (maxOffer == -1)
            {
                show("No one makes an offer, all assests remains in Bank!");
                break;
            }
            else
                i = 0;
        }

        // rule out any player who is already bankrupted
        if (allPlayers[copyPlayerOrder[i]]->getIsBankrupt())
        {
            ++i;
            continue;
        }

        // let the current player bid
        std::string pyn = copyPlayerOrder[i];
        if (pyn == maxOfferer)
        {
            show("The bid ends with max offer of " + std::to_string(maxOffer) + " from " + maxOfferer);
            break;
        }
        show("Current bidding Player: " + pyn + " with highest offer:  " + std::to_string(maxOffer) + " from " + maxOfferer);
        show("How much would you offer? ( input any number <= current max offer if you decide to withdraw ) ");
        int offer = 0;
        // read in offer
        while (true)
        {
            if (!(min >> offer))
            {
                if (min.eof())
                    break;
                min.clear();
                min.ignore();
                show("Your command is invalid! Please put in an integer: ");
            }
            else
            {
                // withdraw
                if (offer <= maxOffer)
                {
                    copyPlayerOrder.erase(copyPlayerOrder.begin() + i);
                    --i;
                    break;
                }
                if (allPlayers[pyn]->getDebt() > 0 || offer > allPlayers[pyn]->getMoney())
                    show("You don't have that much money left! Please enter again: ( input any number <= current max offer if you decide to skip ) ");
                else
                    break;
            }
        }
        // offer higher
        if (offer > maxOffer)
        {
            show("You have offered a higher amount at: " + std::to_string(offer));
            maxOffer = offer;
            maxOfferer = pyn;
        }
        else
        {
            show("You withdrew from this auction.");
        }
        ++i;
    }

    return std::make_pair(maxOfferer, maxOffer);
}

ModelImplPrimitive::ModelImplPrimitive(std::istream &tin, std::ostream &tout)
    : ModelImplementation(tin, tout) {}

bool ModelImplPrimitive::playerProceed(const std::string &pn, int steps)
{
    if (!existPlayer(pn))
        return false;

    std::shared_ptr<Player> p = allPlayers[pn];
    bool prevJail = p->getIsJailed();
    int prevPos = p->getPosition();

    if (p->getIsJailed())
    {
        // if is Jailed, must be at DC Tims Line, visit directly
        strategies[board->getSquareLocation("DC Tims Line")]->acceptVisitor(p, board, min, mout);
    }
    else
    {
        // not Jailed, then proceed according to steps
        p->moveForward(steps, board->getTotalSquareNum());
        prevPos = p->getPosition();
        playerPayVisit(p);
    }

    // check with the position before the strategy applied, if not jailed and position moved, means need to apply another strategy
    while (prevPos != p->getPosition() && !(p->getIsJailed()))
    {
        prevPos = p->getPosition();
        playerPayVisit(p);
    }
    // return this
    return allPlayers[pn]->getIsJailed() || prevJail;
}

void ModelImplPrimitive::gotoTims(const std::string &pn)
{
    if (!existPlayer(pn))
        return;
    allPlayers[pn]->setIsJailed(true);
    allPlayers[pn]->setNumJailed(0);

    int timsLocation = board->getSquareLocation("DC Tims Line");
    int playerLocation = allPlayers[pn]->getPosition();
    int totalSquares = board->getTotalSquareNum();
    int moveAmount = (timsLocation - playerLocation + totalSquares) % totalSquares;
    allPlayers[pn]->moveForward(moveAmount, totalSquares);
}

void ModelImplPrimitive::trade(const std::string &pn1, const std::string &pn2, const std::string &property, int price, bool receiver)
{
    show("This is a very primitive version of the game, so you are not allowed to trade.");
}

void ModelImplPrimitive::trade(const std::string &pn1, const std::string &pn2, const std::string &property1, const std::string &property2)
{
    show("This is a very primitive version of the game, so you are not allowed to trade.");
}

void ModelImplPrimitive::improve(const std::string &pn, const std::string &property, bool action)
{
    show("This is a very primitive version of the game, so you are not allowed to improve.");
}

void ModelImplPrimitive::mortgage(const std::string &pn, const std::string &property, bool action)
{
    show("This is a very primitive version of the game, so you are not allowed to mortgage.");
}

bool ModelImplPrimitive::bankrupt(const std::string &pn, std::string &doubleKill)
{
    if (!existPlayer(pn))
        return false;
    if (allPlayers[pn]->getDebt() > 0)
    {
        std::string pyn;
        show("You are able to bankrupt, preparing to drop out!");
        allPlayers[pn]->dropOut();
        if (allPlayers[pn]->getDebtOwner() == "BANK")
        {
            show("Your properties are being auctioned!");
            auctionPlayer(pn);
        }
        else
        {
            show("Your properties are being transfered to Debtor: " + allPlayers[pn]->getDebtOwner());
            pyn = allPlayers[pn]->getDebtOwner();
            // transfer cup
            allPlayers[pyn]->setNumCups(allPlayers[pyn]->getNumCups() + allPlayers[pn]->getNumCups());
            allPlayers[pn]->setNumCups(0);
            // transfer money
            allPlayers[pyn]->setMoney(allPlayers[pyn]->getMoney() + allPlayers[pn]->getMoney());
            allPlayers[pn]->setMoney(0);
            // transfer properties
            for (auto &s : board->getAssets(pn))
            {
                std::shared_ptr<Building> b = std::dynamic_pointer_cast<Building>(s);
                if (b.get() == nullptr)
                    std::cerr << "This should never happen: a player owns a nonpropery";

                board->setOwner(b->getName(), allPlayers[pyn]);

                if (b->getIsMortgaged())
                {
                    show("Player " + pyn + " would you like to unmortgage this property: " + b->getName() + "? ( yes/no )");
                    std::string ans;
                    while (true)
                    {
                        if (!(min >> ans))
                        {
                            if (min.eof())
                                break;
                            min.clear();
                            min.ignore();
                            show("Your command is invalid! Please put in \"yes\" or \"no\": ");
                        }
                        else
                        {
                            if (ans == "yes")
                            {
                                deductMoney(allPlayers[pyn], b->getPurchaseCost() * 0.6, "BANK");
                                b->setIsMortgaged(false);
                                break;
                            }
                            else if (ans == "no")
                            {
                                deductMoney(allPlayers[pyn], b->getPurchaseCost() * 0.1, "BANK");
                                break;
                            }
                            else
                                show("Your command is invalid! Please put in \"yes\" or \"no\": ");
                        }
                    }
                }
                board->setOwner(b->getName(), allPlayers[pyn]);
            }
        }

        allPlayers.erase(pn);
        playerOrder.erase(std::remove(playerOrder.begin(), playerOrder.end(), pn), playerOrder.end());

        if (playerOrder.size() == 1)
            throw ModelExcept{"Congratulations! " + playerOrder[0] + " has won!"};

        if (pyn != "" && allPlayers[pyn]->getDebt() > 0)
            doubleKill = pyn;

        return true;
    }
    else
    {
        show("You are not in debt, cannot bankrupt!");
        return false;
    }
}

void ModelImplPrimitive::auctionPlayer(const std::string &pn)
{
    if (!existPlayer(pn))
        return;

    for (auto &s : board->getAssets(pn))
    {
        std::shared_ptr<Building> b = std::dynamic_pointer_cast<Building>(s);
        if (b.get() == nullptr)
            std::cerr << "This should never happen: a player owns a nonpropery";
        b->setIsMortgaged(false);
    }

    show("Auction Start! Bid for Player " + pn);
    show("These are all assets under auction: ");
    std::shared_ptr<Player> p = allPlayers[pn];
    getInfo(pn);

    std::pair<std::string, int> result = auctionHelper();
    std::string maxOfferer = result.first;
    int maxOffer = result.second;

    Player::setTotalNumCups(Player::getTotalNumCups() - allPlayers[pn]->getNumCups());

    if (maxOffer >= 0)
    {
        allPlayers[maxOfferer]->setMoney(allPlayers[maxOfferer]->getMoney() + allPlayers[pn]->getMoney());
        deductMoney(allPlayers[maxOfferer], maxOffer, "BANK");
        // for all property
        for (auto &s : board->getAssets(pn))
        {
            std::shared_ptr<Building> b = std::dynamic_pointer_cast<Building>(s);
            if (b.get() == nullptr)
                std::cerr << "This should never happen: a player owns a nonpropery";

            board->setOwner(b->getName(), allPlayers[maxOfferer]);
        }
    }
    else
    {
        // for all property, give to bank
        for (auto &s : board->getAssets(pn))
        {
            std::shared_ptr<Building> b = std::dynamic_pointer_cast<Building>(s);
            if (b.get() == nullptr)
                std::cerr << "This should never happen: a player owns a nonpropery";
            std::shared_ptr<AcademicBuilding> ab = std::dynamic_pointer_cast<AcademicBuilding>(s);

            if (ab.get() != nullptr)
            {
                ab->setImprovementLevel(0);
            }
            b->setIsMortgaged(false);
            std::shared_ptr<Player> bank;
            board->setOwner(b->getName(), bank);
        }
    }
}

void ModelImplPrimitive::sellBuilding(const std::string &pn, const std::string &bn)
{
    if (!existPlayer(pn))
        return;
    if (!existBuilding(bn))
        return;
    std::shared_ptr<Building> b = std::dynamic_pointer_cast<Building>(board->getSquareBuilding(bn));
    // check valid
    bool checkProperty = (b.get() != nullptr);
    // check building is improvable
    if (!checkProperty)
    {
        show(bn + " is not an ownable property!");
        return;
    }

    getInfo(b);
    show("Do you want to buy this Building?");
    std::string ans;
    while (true)
    {
        if (!(min >> ans))
        {
            if (min.eof())
                break;
            min.clear();
            min.ignore();
            show("Your command is invalid! Please put in yes/no:");
        }
        else
        {
            if (ans == "yes" || ans == "no")
            {
                break;
            }
            else
            {
                show("Your command is invalid! Please put in yes/no:");
            }
        }
    }
    if (ans == "yes")
    {
        if (allPlayers[pn]->getDebt() > 0 || b->getPurchaseCost() > allPlayers[pn]->getMoney())
        {
            show("You don't have that much money left! Will auction!");
            auctionBuilding(b->getName());
        }
        else
        {
            allPlayers[pn]->setMoney(allPlayers[pn]->getMoney() - b->getPurchaseCost());
            board->setOwner(b->getName(), allPlayers[pn]);
        }
    }
    else
    {
        auctionBuilding(b->getName());
    }
}

void ModelImplPrimitive::auctionBuilding(const std::string &bn)
{
    if (!existBuilding(bn))
        return;
    std::shared_ptr<Building> b = std::dynamic_pointer_cast<Building>(board->getSquareBuilding(bn));
    // check valid
    bool checkProperty = (b.get() != nullptr);
    // check building is improvable
    if (!checkProperty)
    {
        show(bn + " is not an ownable property!");
        return;
    }

    // start
    show("Auction Start! Bid for Building " + bn);
    getInfo(b);

    std::pair<std::string, int> result = auctionHelper();
    std::string maxOfferer = result.first;
    int maxOffer = result.second;

    if (maxOffer >= 0)
    {
        // this one will deduct money, player is guarenteed to have enough money, for is checked at auction stage
        deductMoney(allPlayers[maxOfferer], maxOffer, "BANK");
        board->setOwner(b->getName(), allPlayers[maxOfferer]);
    }
    else
    {
        b->setIsMortgaged(false);
        std::shared_ptr<AcademicBuilding> ab = std::dynamic_pointer_cast<AcademicBuilding>(board->getSquareBuilding(bn));
        if (ab.get() != nullptr)
        {
            ab->setImprovementLevel(0);
        }
        std::shared_ptr<Player> bank;
        board->setOwner(b->getName(), bank);
    }
}
