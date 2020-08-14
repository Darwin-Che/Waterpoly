#ifndef _MODELIMPLPRIMITIVE_
#define _MODELIMPLPRIMITIVE_

#include "ModelImplementation.h"

class ModelImplPrimitive : public ModelImplementation
{
    virtual void playerPayVisit(std::shared_ptr<Player> p);

    // deal with interaction loop with Players
    virtual std::pair<std::string, int> auctionHelper();

    // wrapper around auction building
    virtual void sellBuilding(const std::string &pn, const std::string &bn);

public:
    ModelImplPrimitive(std::istream &tin, std::ostream &tout);

    /************** Methods called by Controller **************/

    // call corresponding strategy
    virtual bool playerProceed(const std::string &pn, int steps);

    // directly gotoTims
    virtual void gotoTims(const std::string &pn);

    // playername1 give playername2 property, and get price back
    // noexception, but need to check if the action can be done (bankruptcy)
    virtual void trade(const std::string &pn1, const std::string &pn2, const std::string &property, int price, bool receiver);

    // playername1 give playername2 property1, and get property2 back
    virtual void trade(const std::string &pn1, const std::string &pn2, const std::string &property1, const std::string &property2);

    // action : true is buy, false is sell
    // need to check bankruptcy
    virtual void improve(const std::string &pn, const std::string &property, bool action);

    // action : true is mortgage, false is unmortgage
    // need to check bankruptcy
    virtual void mortgage(const std::string &pn, const std::string &property, bool action);

    // check if the player is able to dropout;
    // if yes, take care of, return true
    // 1. player property and debt; 2. allPlayer vector
    // if no, return false
    // strong exception guarentee
    virtual bool bankrupt(const std::string &pn, std::string &doubleKill);

    virtual void auctionPlayer(const std::string &pn);

    virtual void auctionBuilding(const std::string &bn);
};

#endif
