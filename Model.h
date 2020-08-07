#ifndef _MODEL_
#define _MODEL_

#include <memory>
#include <string>
#include <map>
#include <iostream>
#include "Player.h"

class View;
class Board;
class Player;
class Strategy;
class Square;

class Model
{
    std::map<int, Strategy> strategies;
    std::istream &min;
    std::ostream &mout;
    std::shared_ptr<Board> board;
    std::map<std::string, std::shared_ptr<Player>> allPlayers;
    std::vector<std::string> playerOrder;

    bool squareTradable(std::shared_ptr<Square> s);
    void payDebt(std::shared_ptr<Player> p1);
    bool deductMoney(std::shared_ptr<Player> p, int price, const std::string& debtor);

public:
    Model(std::istream &tin, std::ostream &tout);
    /************** Methods called by Controller **************/

    // let the view show the message, do nothing else
    // provide noexcept guarantee
    void show(const std::string &message) noexcept;

    // check if the player does not own any money
    bool canNext(const std::string &player) noexcept;

    // playername1 give playername2 property, and get price back
    // noexception, but need to check if the action can be done (bankruptcy)
    void trade(const std::string &pn1, const std::string &pn2, const std::string &property, int price) noexcept;

    // playername1 give playername2 property1, and get property2 back
    void trade(const std::string &pn1, const std::string &pn2, const std::string &property1, const std::string &property2) noexcept;

    // action : true is buy, false is sell
    // need to check bankruptcy
    void improve(const std::string &pn, const std::string &property, bool action) noexcept;

    // action : true is mortgage, false is unmortgage
    // need to check bankruptcy
    void mortgage(const std::string &pn, const std::string &property, bool action) noexcept;

    // check if the player is able to dropout; 
    // if yes, take care of, return true
    // 1. player property and debt; 2. allPlayer vector
    // if no, return false
    // strong exception guarentee
    bool bankrupt(const std::string &pn) noexcept;

    void auctionPlayer(const std::string &pn);

    void auctionBuilding(const std::string &pn);

    // check if able and output all player information by view
    void getInfo();

    // check if able and output certain player information by view
    void getInfo(const std::string &pn);

    // save the game progress to out, in the pnorder's order
    // pnorder is a list of player names
    // strong exception guarentee: could throw if ostream cannot be written
    void save(std::ostream &out);
};

#endif
