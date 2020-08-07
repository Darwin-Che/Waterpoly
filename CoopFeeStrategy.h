#ifndef _COOPFEESTRATEGY_H_
#define _COOPFEESTRATEGY_H_
#include "VisitStrategy.h"

class CoopFeeStrategy : public VisitStrategy {
  public:
    // The player will pay $150 coop fee
    virtual void acceptVisitor(std::shared_ptr<Player> player,
            std::shared_ptr<Board> board,
            std::istream& in, std::ostream& out) override;
};

#endif

