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
    board->setOwner(property, pn2);

    payDebt(allPlayers[pn1]);

}

void Model::trade(const std::string &pn1, const std::string &pn2, const std::string &property1, const std::string &property2)
{
    std::shared_ptr<Square> s1 = board->getSquare(property1);
    std::shared_ptr<Square> s2 = board->getSquare(property2);

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
    board->setOwner(property1, pn2);
    board->setOwner(property2, pn1);
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

