#ifndef _MOVESTRATEGY_H_
#define _MOVESTRATEGY_H_
#include "VisitStrategy.h"

class MoveStrategy : public VisitStrategy {
    int moveAmount;

  public:
    MoveStrategy(int moveAmount);
    virtual void acceptVisitor(std::shared_ptr<Player> player,
            std::shared_ptr<Board> board,
            std::istream in, std::ostream out) override;
};

#endif

