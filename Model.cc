#include "Model.h"
#include "ModelImplPrimitive.h"
#include "ModelImplDefault.h"
#include "ModelImplHouseRules.h"

Model::Model(std::istream &tin, std::ostream &tout,
        const std::string &version, std::vector<int> houseRules) {
    if (version == "Primitive")
        pImpl = std::make_shared<ModelImplPrimitive>(tin, tout);
    else if (version == "Default")
        pImpl = std::make_shared<ModelImplDefault>(tin, tout);
    else if (version == "HouseRules") {
        pImpl = std::make_shared<ModelImplHouseRules>(tin, tout, houseRules);
    }
}

    /************** Methods called by Main **************/

// construct playerOrder and allPlayers
void Model::loadPlayer(std::vector<std::shared_ptr<Player>> playerList) {
    pImpl->loadPlayer(playerList);
}

void Model::clearPlayer() {
    pImpl->clearPlayer();
}

    // construct strategies and board
void Model::loadMap(std::shared_ptr<Board> tboard, std::vector<std::shared_ptr<VisitStrategy>> tstrategies) {
    pImpl->loadMap(tboard, tstrategies);
}

void Model::clearMap() {
    pImpl->clearMap();
}

    /************** Methods called by Controller **************/

// true if no debt
bool Model::checkPlayerDebt(const std::string &pn) {
    return pImpl->checkPlayerDebt(pn);
}

// return whether the player was Jailed at the start or end of function
bool Model::playerJailed(const std::string &pn) {
    return pImpl->playerJailed(pn);
}

// call corresponding strategy
bool Model::playerProceed(const std::string &pn, int steps) {
    return pImpl->playerProceed(pn, steps);
}

// directly gotoTims
void Model::gotoTims(const std::string &pn) {
    pImpl->gotoTims(pn);
}

// let the view show the message, do nothing else
// provide noexcept guarantee
void Model::show(const std::string &message) {
    pImpl->show(message);
}

// check if the player does not own any money
std::string Model::nextPlayerName(const std::string &pn, bool force) {
    return pImpl->nextPlayerName(pn, force);
}

// playername1 give playername2 property, and get price back
// noexception, but need to check if the action can be done (bankruptcy)
void Model::trade(const std::string &pn1, const std::string &pn2, const std::string &property, int price, bool receiver) {
    pImpl->trade(pn1, pn2, property, price, receiver);
}

// playername1 give playername2 property1, and get property2 back
void Model::trade(const std::string &pn1, const std::string &pn2, const std::string &property1, const std::string &property2) {
    pImpl->trade(pn1, pn2, property1, property2);
}

// action : true is buy, false is sell
// need to check bankruptcy
void Model::improve(const std::string &pn, const std::string &property, bool action) {
    pImpl->improve(pn, property, action);
}

// action : true is mortgage, false is unmortgage
// need to check bankruptcy
void Model::mortgage(const std::string &pn, const std::string &property, bool action) {
    pImpl->mortgage(pn, property, action);
}

// check if the player is able to dropout;
// if yes, take care of, return true
// 1. player property and debt; 2. allPlayer vector
// if no, return false
// strong exception guarentee
bool Model::bankrupt(const std::string &pn, std::string &doubleKill) {
    return pImpl->bankrupt(pn, doubleKill);
}

void Model::auctionPlayer(const std::string &pn) {
    pImpl->auctionPlayer(pn);
}

void Model::auctionBuilding(const std::string &bn) {
    pImpl->auctionBuilding(bn);
}

// check if able and output all player information by view
void Model::getInfo() {
    pImpl->getInfo();
}

// check if able and output certain player information by view
void Model::getInfo(const std::string &pn) {
    pImpl->getInfo(pn);
}

// save the game progress to out, in the pnorder's order
// pnorder is a list of player names
// strong exception guarentee: could throw if ostream cannot be written
void Model::save(std::ostream &out, std::string pn) {
    pImpl->save(out, pn);
}
