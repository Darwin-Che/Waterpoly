#ifndef _TUITIONSTRATEGY_H_
#define _TUITIONSTRATEGY_H_
#include "VisitStrategy.h"

class TuitionStrategy : public VisitStrategy {
  public:
    // Player will pay $300 tuition or 10% of his/her total networth
    virtual void acceptVisitor(std::shared_ptr<Player> player,
            std::shared_ptr<Board> board,
            std::istream& in, std::ostream& out) override;
};

#endif

