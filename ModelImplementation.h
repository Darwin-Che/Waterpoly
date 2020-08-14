#ifndef _MODELIMPL_
#define _MODELIMPL_

#include <memory>
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include "Player.h"

class View;
class Board;
class Player;
class VisitStrategy;
class Square;

class ModelImplementation
{
  protected:
    std::vector<std::shared_ptr<VisitStrategy>> strategies;
    std::istream &min;
    std::ostream &mout;
    std::shared_ptr<Board> board;
    std::map<std::string, std::shared_ptr<Player>> allPlayers;
    std::vector<std::string> playerOrder;

    // true if can afford
    bool checkPlayerAfford(std::shared_ptr<Player> p, int price);

    // true if owner
    bool checkOwner(std::shared_ptr<Player> p, const std::string &bn);

    bool existPlayer(const std::string &pn);
    bool existBuilding(const std::string &bn); 

    // check if p1 is in debt and can pay debt, if so, paydebt to debtor, otherwise do nothing
    void payDebt(std::shared_ptr<Player> p1);

    // deduct money price from p:
    // if p is not in debt, p can pay price, pay directly
    // if p is not in debt, p cannot pay, add price to debt, take debtor
    // if p is in debt, then debtor should be the same, and add debt up
    bool deductMoney(std::shared_ptr<Player> p, int price, const std::string &debtor);

    // wrapper around s's getInfo, add some current owner,rent information
    virtual void getInfo(std::shared_ptr<Square> s);


public:
    ModelImplementation(std::istream &tin, std::ostream &tout);

    /************** Methods called by Main **************/

    // construct playerOrder and allPlayers
    virtual void loadPlayer(std::vector<std::shared_ptr<Player>> playerList);
    virtual void clearPlayer();

    // construct strategies and board
    virtual void loadMap(std::shared_ptr<Board> tboard, std::vector<std::shared_ptr<VisitStrategy>> tstrategies);
    virtual void clearMap();

    /************** Methods called by Controller **************/

    // true if no debt
    bool checkPlayerDebt(const std::string &pn);

    // return whether the player was Jailed at the start or end of function
    bool playerJailed(const std::string &pn);

    // call corresponding strategy
    virtual bool playerProceed(const std::string &pn, int steps) = 0;

    // directly gotoTims
    virtual void gotoTims(const std::string &pn) = 0;

    // let the view show the message, do nothing else
    // provide noexcept guarantee
    void show(const std::string &message);

    // check if the player does not own any money
    std::string nextPlayerName(const std::string &pn, bool force = false);

    // playername1 give playername2 property, and get price back
    // noexception, but need to check if the action can be done (bankruptcy)
    virtual void trade(const std::string &pn1, const std::string &pn2, const std::string &property, int price, bool receiver) = 0;

    // playername1 give playername2 property1, and get property2 back
    virtual void trade(const std::string &pn1, const std::string &pn2, const std::string &property1, const std::string &property2) = 0;

    // action : true is buy, false is sell
    // need to check bankruptcy
    virtual void improve(const std::string &pn, const std::string &property, bool action) = 0;

    // action : true is mortgage, false is unmortgage
    // need to check bankruptcy
    virtual void mortgage(const std::string &pn, const std::string &property, bool action) = 0;

    // check if the player is able to dropout;
    // if yes, take care of, return true
    // 1. player property and debt; 2. allPlayer vector
    // if no, return false
    // strong exception guarentee
    virtual bool bankrupt(const std::string &pn, std::string &doubleKill) = 0;

    virtual void auctionPlayer(const std::string &pn) = 0;

    virtual void auctionBuilding(const std::string &bn) = 0;

    // check if able and output all player information by view
    void getInfo();

    // check if able and output certain player information by view
    virtual void getInfo(const std::string &pn);

    // save the game progress to out, in the pnorder's order
    // pnorder is a list of player names
    // strong exception guarentee: could throw if ostream cannot be written
    virtual void save(std::ostream &out, std::string pn);
};

#endif
