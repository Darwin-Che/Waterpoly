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

bool Model::existPlayer(const std::string &pn)
{
    if (allPlayers[pn].get() != nullptr)
        return true;
    show(pn + " is not a playername!");
    return false;
}

bool Model::existBuilding(const std::string &bn)
{
    if (board->getSquareBuilding(bn).get() != nullptr)
        return true;
    show(bn + " is not a buildingname!");
    return false;
}

void Model::loadPlayer(std::vector<std::shared_ptr<Player>> playerList)
{
    for (auto &p : playerList)
    {
        playerOrder.emplace_back(p->getName());
        allPlayers[p->getName()] = p;
    }
}

void Model::clearPlayer()
{
    playerOrder.clear();
    allPlayers.clear();
}

void Model::loadMap(std::shared_ptr<Board> tboard, std::vector<std::shared_ptr<VisitStrategy>> tstrategies)
{
    board = tboard;
    strategies = tstrategies;
}

void Model::clearMap()
{
    board = std::shared_ptr<Board>();
    strategies.clear();
}

void Model::payDebt(std::shared_ptr<Player> p1)
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

bool Model::squareTradable(std::shared_ptr<Square> s)
{
    Building *sBuilding = dynamic_cast<Building *>(s.get());
    AcademicBuilding *sAcademic = dynamic_cast<AcademicBuilding *>(s.get());

    bool checkProperty_type = (sBuilding != nullptr);
    if (!checkProperty_type)
    {
        show(s->getName() + " is not an Ownable Bulding!");
        return false;
    }

    bool checkProperty_Mort = (sBuilding->getIsMortgaged() == false);
    if (!checkProperty_Mort)
    {
        show(s->getName() + " must first be unmortgaged!");
        return false;
    }

    bool checkProperty_Impr1 = (sAcademic == nullptr);
    if (checkProperty_Impr1)
        return true;

    bool checkProperty_Impr2 = (sAcademic->getImprovementLevel() == 0);
    if (!checkProperty_Impr2)
    {
        show("All improvement of " + s->getName() + " must first be sold!");
        return false;
    }
    bool checkProperty_Impr3 = (board->inMonopoly(sAcademic->getName()));
    if (!checkProperty_Impr3)
        return true;

    for (auto &ss : board->getMonopoly(sAcademic->getName()))
    {
        AcademicBuilding *ssAcademic = dynamic_cast<AcademicBuilding *>(ss.get());
        if (ssAcademic->getImprovementLevel() != 0)
        {
            show("All improvement of " + ss->getName() + " must first be sold, for they are in the same monopoly block!");
            return false;
        }
    }
    return true;
}

bool Model::deductMoney(std::shared_ptr<Player> p, int price, const std::string &debtor)
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

std::pair<std::string, int> Model::auctionHelper()
{
    std::string maxOfferer;
    int maxOffer = -1;
    int i = 0;

    while (true)
    {
        if (i == playerOrder.size())
        {
            if (maxOffer == -1)
            {
                show("No one makes an offer, all assests go to Bank!");
                break;
            }
            else
            {
                i = 0;
            }
        }
        if (allPlayers[playerOrder[i]]->getIsBankrupt())
        {
            ++i;
            continue;
        }
        std::string pyn = playerOrder[i];
        if (pyn == maxOfferer)
        {
            show("The bid ends with max offer of " + std::to_string(maxOffer) + " from " + maxOfferer);
            break;
        }
        show("Current bidding Player: " + pyn + " with highest offer: " + std::to_string(maxOffer) + " from " + maxOfferer);
        show("How much would you offer? ( input \"-1\" if you decide to skip ) ");
        int offer;
        while (true)
        {
            if (!(min >> offer))
            {
                if (min.eof())
                    break;
                min.clear();
                min.ignore();
                show("Your command is invalid! Please put in an integer:");
            }
            else
            {
                if (allPlayers[pyn]->getDebt() > 0 || offer > allPlayers[pyn]->getMoney())
                {
                    show("You don't have that much money left! Please enter again: ( input \"-1\" if you decide to skip ) ");
                }
                else
                {
                    break;
                }
            }
        }
        if (offer > maxOffer)
        {
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
    else
    {
    }
}

Model::Model(std::istream &tin, std::ostream &tout)
    : min{tin}, mout{tout}
{
}

void Model::playerProceed(const std::string &pn, int steps)
{
    if (!existPlayer(pn))
        return;

    std::shared_ptr<Player> p = allPlayers[pn];
    if (p->getIsJailed())
    {
        strategies[board->getSquareLocation("DC Tims Line")]->acceptVisitor(p, board, min, mout);
    }
    else
    {
        p->setPosition((p->getPosition() + steps) % board->getTotalSquareNum());
        strategies[board->getSquareLocation("COLLECT OSAP")]->acceptVisitor(p, board, min, mout);
        int prevPos = p->getPosition();
        if (board->getOwner(board->getSquare(p->getPosition())->getName()).get() == nullptr && std::dynamic_pointer_cast<Building>(board->getSquare(p->getPosition())).get() != nullptr)
        {
            sellBuilding(pn, board->getSquare(p->getPosition())->getName());
        }
        else
        {
            strategies[p->getPosition()]->acceptVisitor(p, board, min, mout);
        }

        while (prevPos != p->getPosition() && !(p->getIsJailed()))
        {
            strategies[board->getSquareLocation("COLLECT OSAP")]->acceptVisitor(p, board, min, mout);
            prevPos = p->getPosition();
            if (board->getOwner(board->getSquare(p->getPosition())->getName()).get() == nullptr && std::dynamic_pointer_cast<Building>(board->getSquare(p->getPosition())).get() != nullptr)
            {
                sellBuilding(pn, board->getSquare(p->getPosition())->getName());
            }
            else
            {
                strategies[p->getPosition()]->acceptVisitor(p, board, min, mout);
            }
        }
    }
}

void Model::gotoTims(const std::string &pn)
{
    if (!existPlayer(pn))
        return;

    allPlayers[pn]->setIsJailed(true);
    allPlayers[pn]->setNumJailed(0);
    allPlayers[pn]->setPosition(board->getSquareLocation("DC Tims Line"));
}

void Model::show(const std::string &message)
{
    mout << message << std::endl;
}

std::string Model::nextPlayerName(const std::string &pn, bool force)
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

bool Model::askTrade(const std::string &pn)
{
    if (!existPlayer(pn))
        return false;

    show("Do you want to trade? " + pn);
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
                break;
            else
                show("Your command is invalid! Please put in yes/no:");
        }
    }
    if (ans == "yes")
        return true;
    else
        return false;
}

void Model::trade(const std::string &pn1, const std::string &pn2, const std::string &property, int price, bool receiver)
{
    if (!existPlayer(pn1))
        return;
    if (!existPlayer(pn2))
        return;
    if (!existBuilding(property))
        return;

    std::shared_ptr<Square> s = board->getSquareBuilding(property);

    // checking process
    bool checkP1 = (board->getOwner(property) == allPlayers[pn1]);
    if (!checkP1)
    {
        show(property + " is not owned by " + pn1);
        return;
    }
    bool checkP2_debt = (allPlayers[pn2]->getDebt() == 0);
    if (!checkP2_debt)
    {
        show(pn2 + " is in debt, cannot trade before pays off debt!");
        return;
    }
    bool checkP2_money = (allPlayers[pn2]->getMoney() >= price);
    if (!checkP2_money)
    {
        show(pn2 + " does not have enough money to this offer!");
        return;
    }

    if (!squareTradable(s))
        return;

    // ask receiver
    bool result = askTrade(receiver ? pn1 : pn2);
    if (result)
    {
        // trade
        allPlayers[pn1]->setMoney(allPlayers[pn1]->getMoney() + price);
        allPlayers[pn2]->setMoney(allPlayers[pn2]->getMoney() - price);
        board->setOwner(property, allPlayers[pn2]);

        payDebt(allPlayers[pn1]);
    }
    else
    {
        show("Trade offer is rejected!");
    }
}

void Model::trade(const std::string &pn1, const std::string &pn2, const std::string &property1, const std::string &property2)
{
    if (!existPlayer(pn1))
        return;
    if (!existPlayer(pn2))
        return;
    if (!existBuilding(property1))
        return;
    if (!existBuilding(property2))
        return;
    std::shared_ptr<Square> s1 = board->getSquareBuilding(property1);
    std::shared_ptr<Square> s2 = board->getSquareBuilding(property2);

    // checking process
    bool checkP1_prop = (board->getOwner(property1) == allPlayers[pn1]);
    if (!checkP1_prop)
    {
        show(property1 + " is not owned by " + pn1);
        return;
    }

    bool checkP2_prop = (board->getOwner(property2) == allPlayers[pn2]);
    if (!checkP2_prop)
    {
        show(property2 + " is not owned by " + pn2);
        return;
    }

    bool checkP1_debt = (allPlayers[pn1]->getDebt() == 0);
    if (!checkP1_debt)
    {
        show(pn1 + " is in debt, cannot trade before pays off debt!");
        return;
    }

    bool checkP2_debt = (allPlayers[pn2]->getDebt() == 0);
    if (!checkP2_debt)
    {
        show(pn2 + " is in debt, cannot trade before pays off debt!");
        return;
    }

    if (!squareTradable(s1))
        return;
    if (!squareTradable(s2))
        return;

    // ask receiver
    bool result = askTrade(pn2);
    if (result)
    {
        // trade
        board->setOwner(property1, allPlayers[pn2]);
        board->setOwner(property2, allPlayers[pn1]);

        payDebt(allPlayers[pn1]);
    }
    else
    {
        show("Trade offer is rejected!");
    }
}

void Model::improve(const std::string &pn, const std::string &property, bool action)
{
    if (!existPlayer(pn))
        return;

    if (!existBuilding(property))
        return;
    std::shared_ptr<Square> s = board->getSquareBuilding(property);
    std::shared_ptr<AcademicBuilding> sAcademic = std::dynamic_pointer_cast<AcademicBuilding>(s);
    // check valid
    bool checkProperty_Impr = (sAcademic.get() != nullptr);
    // check building is improvable
    if (!checkProperty_Impr)
    {
        show(property + " is not an improvable Square!");
        return;
    }
    // check owner
    bool checkProperty_pn = (board->getOwner(property) == allPlayers[pn]);
    // check building is owned by pn
    if (!checkProperty_pn)
    {
        show(property + " is not even owned by you!");
        return;
    }
    // check building is in monopoly
    if (!(board->inMonopoly(property)))
    {
        show(property + "'s Monopoly block is not entirely owned by you!");
        return;
    }
    // improve
    if (action)
    {
        // check if player is in debt
        if (allPlayers[pn]->getDebt() > 0)
        {
            show("You are still in Debt! First pay off your debt then improve the building!");
        }
        // check improvement is at maximum
        if (sAcademic->getImprovementLevel() > 4)
        {
            show(property + "'s improvement level is already at maximum!");
            return;
        }
        int cost = sAcademic->getImprovementCost();
        // check if player can afford
        if (allPlayers[pn]->getMoney() < cost)
        {
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
        if (sAcademic->getImprovementLevel() < 1)
        {
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
    if (!existPlayer(pn))
        return;
    if (!existBuilding(property))
        return;
    std::shared_ptr<Square> s = board->getSquareBuilding(property);
    std::shared_ptr<Building> b = std::dynamic_pointer_cast<Building>(s);
    // check valid
    bool checkProperty = (b.get() != nullptr);

    if (!checkProperty)
    {
        show(property + " is not an ownable property!");
        return;
    }
    // check owner
    bool checkProperty_pn = (board->getOwner(property) == allPlayers[pn]);
    // check building is owned by pn
    if (!checkProperty_pn)
    {
        show(property + " is not even owned by you!");
        return;
    }

    if (!squareTradable(board->getSquareBuilding(property)))
        return;

    // improve
    if (action)
    {
        // check status
        if (b->getIsMortgaged())
        {
            show(property + " is already mortgaged!");
            return;
        }
        std::shared_ptr<AcademicBuilding> ab = std::dynamic_pointer_cast<AcademicBuilding>(s);
        if (ab->getImprovementLevel() != 0)
        {
            show("You must first sell all improvement on this building!");
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
        if (allPlayers[pn]->getDebt() > 0)
        {
            show("You are still in Debt! First pay off your debt then unmortgage the building!");
        }
        // check improvement is at maximum
        if (!b->getIsMortgaged())
        {
            show(property + " is already unmortgaged!");
            return;
        }
        int cost = b->getPurchaseCost() * 0.6;
        // check if player can afford
        if (allPlayers[pn]->getMoney() < cost)
        {
            show("Your money ( " + std::to_string(allPlayers[pn]->getMoney()) + " ) is insufficient for unmortgagement cost ( " + std::to_string(cost) + " )!");
            return;
        }
        // execute change
        b->setIsMortgaged(false);
        allPlayers[pn]->setMoney(allPlayers[pn]->getMoney() - cost);
    }
}

bool Model::bankrupt(const std::string &pn, std::string &doubleKill)
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

void Model::auctionPlayer(const std::string &pn)
{
    if (!existPlayer(pn))
        return;

    show("Auction Start! Bid for Player " + pn);
    show("These are all assets under auction: ");
    std::shared_ptr<Player> p = allPlayers[pn];
    getInfo(pn);

    std::pair<std::string, int> result = auctionHelper();
    std::string maxOfferer = result.first;
    int maxOffer = result.second;

    if (maxOffer >= 0)
    {
        deductMoney(allPlayers[maxOfferer], maxOffer, "BANK");
        // for all property
        for (auto &s : board->getAssets(pn))
        {
            std::shared_ptr<Building> b = std::dynamic_pointer_cast<Building>(s);
            if (b.get() == nullptr)
                std::cerr << "This should never happen: a player owns a nonpropery";
            b->setIsMortgaged(false);

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

void Model::sellBuilding(const std::string &pn, const std::string &bn)
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

void Model::auctionBuilding(const std::string &bn)
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

void Model::getInfo()
{
    for (auto &p : playerOrder)
    {
        getInfo(p);
    }
}

void Model::getInfo(const std::string &pn)
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

void Model::save(std::ostream &out, std::string pn)
{
    if (allPlayers[pn]->getDebt() > 0) {
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
        if (i == playerOrder.end()) i = playerOrder.begin();
        if (*i == pn) break;
    }
    out << board->saveInfo();
}
