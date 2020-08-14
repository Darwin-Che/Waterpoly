#include "ModelImplDefault.h"
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

bool ModelImplDefault::squareTradable(std::shared_ptr<Square> s)
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

bool ModelImplDefault::getMonopolyMortgage(std::shared_ptr<Square> s){
    auto neighbours = board->getMonopoly(s->getName());
    for( auto nSquare: neighbours){
        auto building = std::dynamic_pointer_cast<Building>(nSquare);
        if( building->getIsMortgaged()){
            return true;
        }
    }
    return false;
}

ModelImplDefault::ModelImplDefault(std::istream &tin, std::ostream &tout)
    : ModelImplPrimitive(tin, tout)
{
}

bool ModelImplDefault::askTrade(const std::string &pn)
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

void ModelImplDefault::trade(const std::string &pn1, const std::string &pn2, const std::string &property, int price, bool receiver)
{
    if (!existPlayer(pn1))
        return;
    if (!existPlayer(pn2))
        return;
    if (!existBuilding(property))
        return;

    std::shared_ptr<Square> s = board->getSquareBuilding(property);

    // checking process
    if (!checkOwner(allPlayers[pn1], property) || !checkPlayerDebt(pn2) || !checkPlayerAfford(allPlayers[pn2], price))
        return;

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

void ModelImplDefault::trade(const std::string &pn1, const std::string &pn2, const std::string &property1, const std::string &property2)
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
    if (!checkOwner(allPlayers[pn1], property1) || !checkOwner(allPlayers[pn2], property2) || !checkPlayerDebt(pn1) || !checkPlayerDebt(pn2))
        return;

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

void ModelImplDefault::improve(const std::string &pn, const std::string &property, bool action)
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
    if (!checkOwner(allPlayers[pn], property))
        return;

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
        if (!checkPlayerDebt(pn))
            return;
        // check improvement is at maximum
        if (sAcademic->getImprovementLevel() > 4)
        {
            show(property + "'s improvement level is already at maximum!");
            return;
        }
        // check if the the building is mortgaged
        if (getMonopolyMortgage(sAcademic)){
            show(property + "is mortgaged, you cannot improve a mortgaged building!");
            return;
        }
        int cost = sAcademic->getImprovementCost();
        // check if player can afford
        if (!checkPlayerAfford(allPlayers[pn], cost))
            return;

        // execute change
        sAcademic->setImprovementLevel(sAcademic->getImprovementLevel() + 1);
        allPlayers[pn]->setMoney(allPlayers[pn]->getMoney() - cost);
        show(property + "'s improvement level successfully increases to "
        +std::to_string(sAcademic->getImprovementLevel())+".");
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
        show(property + "'s improvement level successfully decreases to "
        +std::to_string(sAcademic->getImprovementLevel())+".");
        // see if debt can be paid
        payDebt(allPlayers[pn]);
    }
}

void ModelImplDefault::mortgage(const std::string &pn, const std::string &property, bool action)
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
    if (!checkOwner(allPlayers[pn], property))
        return;

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
        if ( ab!=nullptr && ab->getImprovementLevel() != 0)
        {
            show("You must first sell all improvement on this building!");
            return;
        }
        int refund = b->getPurchaseCost() * 0.5;
        // execute change
        b->setIsMortgaged(true);
        allPlayers[pn]->setMoney(allPlayers[pn]->getMoney() + refund);
        show(property + "is successfully mortgaged.");
        // see if debt can be paid
        payDebt(allPlayers[pn]);
    }
    else
    {
        // check if player is in debt
        if (!checkPlayerDebt(pn))
            return;
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
        show(property + "is successfully unmortgaged.");
    }
}

