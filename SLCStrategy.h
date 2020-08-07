#ifndef _SLCSTRATEGY_H_
#define _SLCSTRATEGY_H_
#include "VisitStrategy.h"
#include <vector>

class SLCStrategy : public VisitStrategy {
    std::vector<std::shared_ptr<VisitStrategy>> strategies;

  public:
    SLCStrategy();
    virtual void acceptVisitor(std::shared_ptr<Player> player,
            std::shared_ptr<Board> board,
            std::istream in, std::ostream out) override;
};

#endif

