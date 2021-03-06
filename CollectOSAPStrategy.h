#ifndef _COLLECTOSAPSTRATEGY_H_
#define _COLLECTOSAPSTRATEGY_H_
#include "VisitStrategy.h"

class CollectOSAPStrategy : public VisitStrategy {
  public:
    // The player will collect $200 OSAP fund
    virtual void acceptVisitor(std::shared_ptr<Player> player,
            std::shared_ptr<Board> board,
            std::istream& in, std::ostream& out) override;
};

#endif

