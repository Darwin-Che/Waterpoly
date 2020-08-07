#ifndef _MONEYSTRATEGY_H_
#define _MONEYSTRATEGY_H_
#include "VisitStrategy.h"

class MoneyStrategy : public VisitStrategy {
    int gainAmount;

  public:
    MoneyStrategy(int gainAmount);
    // Player gain money equal to the private field gainAmount
    virtual void acceptVisitor(std::shared_ptr<Player> player,
            std::shared_ptr<Board> board,
            std::istream& in, std::ostream& out) override;
};

#endif

