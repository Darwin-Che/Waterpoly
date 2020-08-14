#ifndef _MODELIMPLDEFAULT_
#define _MODELIMPLDEFAULT_

#include "ModelImplPrimitive.h"

class ModelImplDefault : public ModelImplPrimitive
{
protected:
    // check if the square is a building and is un mortgaged and does not have improvement
    // return true is so, falso otherwise
    virtual bool squareTradable(std::shared_ptr<Square> s);

    virtual bool squareImprovable(const std::string &pn, const std::string &property);

    // return a mortgaged building the monopoly of s
    virtual std::string getMonopolyMortgage(std::shared_ptr<Square> s);
    
    virtual bool askTrade(const std::string &pn);

public:
    ModelImplDefault(std::istream &tin, std::ostream &tout);

    /************** Methods called by Controller **************/

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
};

#endif
