#ifndef _RESIDENCESTRATEGY_H_
#define _RESIDENCESTRATEGY_H_
#include "VisitStrategy.h"

class ResidenceStrategy : public VisitStrategy {
  public:
    virtual void acceptVisitor(std::shared_ptr<Player> player,
            std::shared_ptr<Board> board,
            std::istream in, std::ostream out) override;
};

#endif

