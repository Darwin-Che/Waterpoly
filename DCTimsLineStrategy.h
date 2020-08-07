#ifndef _DCTIMSLINESTRATEGY_H_
#define _DCTIMSLINESTRATEGY_H_
#include "VisitStrategy.h"

class DCTimsLineStrategy : public VisitStrategy {
    int roll1, roll2;
    int roll();
    void rollDouble(std::shared_ptr<Player> player, std::ostream out);
    void payFee(std::shared_ptr<Player> player, std::ostream out);
    void payCup(std::shared_ptr<Player> player, std::ostream out);

  public:
    DCTimsLineStrategy();
    virtual void acceptVisitor(std::shared_ptr<Player> player,
            std::shared_ptr<Board> board,
            std::istream in, std::ostream out) override;
};

#endif

