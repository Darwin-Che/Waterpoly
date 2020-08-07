#ifndef _VISITSTRATEGY_H_
#define _VISITSTRATEGY_H_
#include <string>
#include <memory>
#include <iostream>
#include "Player.h"
#include "Square.h"
#include "Board.h"

class VisitStrategy {
  public:
    virtual void acceptVisitor(std::shared_ptr<Player> player,
            std::shared_ptr<Board> board,
            std::istream in, std::ostream out) = 0;
};

#endif

