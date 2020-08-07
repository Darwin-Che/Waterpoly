#ifndef _MONEYSTRATEGY_H_
#define _MONEYSTRATEGY_H_
#include "VisitStrategy.h"

class MoneyStrategy : public VisitStrategy {
    int changeAmount;

  public:
    MoneyStrategy(int changeAmount);
    virtual void acceptVisitor(std::shared_ptr<Player> player,
            std::shared_ptr<Board> board,
            std::istream in, std::ostream out) override;
};

#endif

