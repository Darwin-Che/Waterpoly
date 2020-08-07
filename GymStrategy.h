#ifndef _GYMSTRATEGY_H_
#define _GYMSTRATEGY_H_
#include "VisitStrategy.h"

class GymStrategy : public VisitStrategy {
  public:
    virtual void acceptVisitor(std::shared_ptr<Player> player,
            std::shared_ptr<Board> board,
            std::istream in, std::ostream out) override;
};

#endif

