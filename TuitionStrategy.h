#ifndef _TUITIONSTRATEGY_H_
#define _TUITIONSTRATEGY_H_
#include "VisitStrategy.h"

class TuitionStrategy : public VisitStrategy {
  public:
    virtual void acceptVisitor(std::shared_ptr<Player> player,
            std::shared_ptr<Board> board,
            std::istream in, std::ostream out) override;
};

#endif

