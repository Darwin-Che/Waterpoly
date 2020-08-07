#ifndef _NEEDLESHALLSTRATEGY_H_
#define _NEEDLESHALLSTRATEGY_H_
#include "VisitStrategy.h"
#include <vector>

class NeedlesHallStrategy : public VisitStrategy {
    // A random strategy will be chosen from this vector and applied to the player
    std::vector<std::shared_ptr<VisitStrategy>> strategies;

  public:
    NeedlesHallStrategy();
    // The player will gain or lose a random amount of money
    virtual void acceptVisitor(std::shared_ptr<Player> player,
            std::shared_ptr<Board> board,
            std::istream& in, std::ostream& out) override;
};

#endif

