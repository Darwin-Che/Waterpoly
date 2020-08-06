#include "Model.h"
#include "View.h"
#include "Player.h"
#include "Square.h"
#include "Building.h"
#include "Gym.h"
#include "AcademicBuilding.h"
#include "Residence.h"
#include <vector>
#include <map>
#include <string>

void Model::payDebt(std::shared_ptr<Player> p1)
{
    if (p1->canPayDebt())
    {
        std::shared_ptr<Player> p2 = allPlayers[p1->getDebtOwner()];
        int amount = p1->getDebt();
        p1->setMoney(p1->getMoney() - amount);
        p2->setMoney(p2->getMoney() + amount);
        p1->setDebt(0);
        p1->setDebtOwner("");
    }
}

Model::Model(std::istream &tin, std::ostream &tout)
    : min{ tin }, mout{ tout }
{
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
    std::shared_ptr<Square> s = board->getSquare(property);

    // checking process 
    bool checkP1 = (board->getOwner(property) == allPlayers[pn2]);
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

    Building* sBuilding = dynamic_cast<Building*>(s.get());
    AcademicBuilding* sAcademic = dynamic_cast<AcademicBuilding*>(s.get());

    bool checkProperty_type = (sBuilding != nullptr);
    if (!checkProperty_type) {
        show(property + " is not an Ownable Bulding!");
        return;
    }

    bool checkProperty_Mort = (sBuilding->getIsMortgaged() == false);
    if (!checkProperty_Mort) {
        show(pn1 + " must first unmortgage this Building!");
        return;
    }

    bool checkProperty_Impr = (sAcademic != nullptr) || (sAcademic->getImprovementLevel() == 0);
    if (!checkProperty_Impr) {
        show(pn1 + " must first sell all improvement on the Building!");
        return;
    }

    // trade
    allPlayers[pn1]->setMoney(allPlayers[pn1]->getMoney() + price);
    allPlayers[pn2]->setMoney(allPlayers[pn2]->getMoney() - price);
    board->setOwner(property, pn2);

    payDebt(allPlayers[pn1]);

}

void Model::trade(const std::string &pn1, const std::string &pn2, const std::string &property1, const std::string &proptery2)
{

}

void Model::improve(const std::string &pn, const std::string &property, bool action)
{

}

void Model::bankrupt(const std::string &pn)
{

}

void Model::getInfo()
{

}

void Model::getInfo(const std::string &pn)
{

}

void Model::save(std::ostream out, std::vector<std::string> pnorder)
{

}

