#ifndef _MODEL_
#define _MODEL_

#include <memory>
#include <string>
#include <map>
#include <vector>
#include <iostream>

class View;
class Board;
class Player;
class VisitStrategy;
class Square;
class ModelImplementation;

class Model
{
    // Bridge design pattern: pointer to implementation class
    std::shared_ptr<ModelImplementation> pImpl;

public:
    Model(std::istream &tin, std::ostream &tout, const std::string &version,
            std::vector<int> houseRules);

    /************** Methods called by Main **************/

    // construct playerOrder and allPlayers
    void loadPlayer(std::vector<std::shared_ptr<Player>> playerList);
    void clearPlayer();

    // construct strategies and board
    void loadMap(std::shared_ptr<Board> tboard, std::vector<std::shared_ptr<VisitStrategy>> tstrategies);
    void clearMap();

    /************** Methods called by Controller **************/

    // true if no debt
    bool checkPlayerDebt(const std::string &pn);

    // return whether the player was Jailed at the start or end of function
    bool playerJailed(const std::string &pn);

    // call corresponding strategy
    bool playerProceed(const std::string &pn, int steps);

    // directly gotoTims
    void gotoTims(const std::string &pn);

    // let the view show the message, do nothing else
    // provide noexcept guarantee
    void show(const std::string &message);

    // check if the player does not own any money
    std::string nextPlayerName(const std::string &pn, bool force = false);

    // playername1 give playername2 property, and get price back
    // noexception, but need to check if the action can be done (bankruptcy)
    void trade(const std::string &pn1, const std::string &pn2, const std::string &property, int price, bool receiver);

    // playername1 give playername2 property1, and get property2 back
    void trade(const std::string &pn1, const std::string &pn2, const std::string &property1, const std::string &property2);

    // action : true is buy, false is sell
    // need to check bankruptcy
    void improve(const std::string &pn, const std::string &property, bool action);

    // action : true is mortgage, false is unmortgage
    // need to check bankruptcy
    void mortgage(const std::string &pn, const std::string &property, bool action);

    // check if the player is able to dropout;
    // if yes, take care of, return true
    // 1. player property and debt; 2. allPlayer vector
    // if no, return false
    // strong exception guarentee
    bool bankrupt(const std::string &pn, std::string &doubleKill);

    void auctionPlayer(const std::string &pn);

    void auctionBuilding(const std::string &bn);

    // check if able and output all player information by view
    void getInfo();

    // check if able and output certain player information by view
    void getInfo(const std::string &pn);

    // save the game progress to out, in the pnorder's order
    // pnorder is a list of player names
    // strong exception guarentee: could throw if ostream cannot be written
    void save(std::ostream &out, std::string pn);
};

#endif
