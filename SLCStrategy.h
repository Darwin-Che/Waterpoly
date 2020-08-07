#ifndef _SLCSTRATEGY_H_
#define _SLCSTRATEGY_H_
#include "VisitStrategy.h"
#include <vector>

class SLCStrategy : public VisitStrategy {
    // A random strategies will be chosen from this vector and applied to the player
    std::vector<std::shared_ptr<VisitStrategy>> strategies;

  public:
    SLCStrategy();
    // The player will move forward or backward a random amount
    virtual void acceptVisitor(std::shared_ptr<Player> player,
            std::shared_ptr<Board> board,
            std::istream& in, std::ostream& out) override;
};

#endif

