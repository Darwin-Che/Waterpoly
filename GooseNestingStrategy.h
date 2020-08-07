#ifndef _GOOSENESTINGSTRATEGY_H_
#define _GOOSENESTINGSTRATEGY_H_
#include "VisitStrategy.h"

class GooseNestingStrategy : public VisitStrategy {
  public:
    virtual void acceptVisitor(std::shared_ptr<Player> player,
            std::shared_ptr<Board> board,
            std::istream in, std::ostream out) override;
};

#endif

