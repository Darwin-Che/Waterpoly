#ifndef _MODELIMPLHOUSERULES_
#define _MODELIMPLHOUSERULES_

#include "ModelImplDefault.h"

class ModelImplHouseRules : public ModelImplDefault
{
    int amountCollected;
    bool option12Activated, option3Activated, option4Activated;

protected:
    virtual void applyStrategy(std::shared_ptr<Player> p) override;
    virtual bool squareImprovable(const std::string &pn, const std::string &property, bool action) override;
    
public:
    // Option 1, 2 and 3 are the 3 house rules mentioned in watopoly.pdf
    // Option 4 is the "even build" rule in the classic Monopoly
    ModelImplHouseRules(std::istream &tin, std::ostream &tout, std::vector<int> houseRuleOptions);

};

#endif
