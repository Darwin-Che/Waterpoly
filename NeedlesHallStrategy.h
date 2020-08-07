#ifndef _NEEDLESHALLSTRATEGY_H_
#define _NEEDLESHALLSTRATEGY_H_
#include "VisitStrategy.h"
#include <vector>

class NeedlesHallStrategy : public VisitStrategy {
    std::vector<std::shared_ptr<VisitStrategy>> strategies;

  public:
    NeedlesHallStrategy();
    virtual void acceptVisitor(std::shared_ptr<Player> player,
            std::shared_ptr<Board> board,
            std::istream in, std::ostream out) override;
};

#endif

