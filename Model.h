#ifndef _MODEL_
#define _MODEL_

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

class Model
{
    std::vector<std::shared_ptr<VisitStrategy>> strategies;
    std::istream &min;
    std::ostream &mout;
    std::shared_ptr<Board> board;
    std::map<std::string, std::shared_ptr<Player>> allPlayers;
    std::vector<std::string> playerOrder;

    bool existPlayer(const std::string & pn);
    bool existBuilding(const std::string &bn);

    // check if the square is a building and is un mortgaged and does not have improvement
    // return true is so, falso otherwise
    bool squareTradable(std::shared_ptr<Square> s);

    // check if p1 is in debt and can pay debt, if so, paydebt to debtor, otherwise do nothing
    void payDebt(std::shared_ptr<Player> p1);

    // deduct money price from p:
    // if p is not in debt, p can pay price, pay directly
    // if p is not in debt, p cannot pay, add price to debt, take debtor
    // if p is in debt, then debtor should be the same, and add debt up
    bool deductMoney(std::shared_ptr<Player> p, int price, const std::string &debtor);

    // deal with interaction loop with Players
    std::pair<std::string, int> auctionHelper();

    // wrapper around s's getInfo, add some current owner,rent information
    void getInfo(std::shared_ptr<Square> s);

    // wrapper around auction building
    void sellBuilding(const std::string &pn, const std::string &bn);

    bool askTrade(const std::string &pn);

public:
    Model(std::istream &tin, std::ostream &tout);

    /************** Methods called by Main **************/

    // construct playerOrder and allPlayers
    void loadPlayer(std::vector<std::shared_ptr<Player>> playerList);
    void clearPlayer();

    // construct strategies and board
    void loadMap(std::shared_ptr<Board> tboard, std::vector<std::shared_ptr<VisitStrategy>> tstrategies);
    void clearMap();

    /************** Methods called by Controller **************/

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
