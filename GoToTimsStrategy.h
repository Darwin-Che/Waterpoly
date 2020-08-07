#ifndef _GOTOTIMSSTRATEGY_H_
#define _GOTOTIMSSTRATEGY_H_
#include "VisitStrategy.h"

class GoToTimsStrategy : public VisitStrategy {
  public:
    // The player will be sent to DC Tims Line
    virtual void acceptVisitor(std::shared_ptr<Player> player,
            std::shared_ptr<Board> board,
            std::istream& in, std::ostream& out) override;
};

#endif

