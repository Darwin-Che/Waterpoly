#ifndef _RESIDENCESTRATEGY_H_
#define _RESIDENCESTRATEGY_H_
#include "VisitStrategy.h"

class ResidenceStrategy : public VisitStrategy {
  public:
    // Player will pay rent to the residence's owner if applicable
    virtual void acceptVisitor(std::shared_ptr<Player> player,
            std::shared_ptr<Board> board,
            std::istream& in, std::ostream& out) override;
};

#endif

