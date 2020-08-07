#ifndef _VISITSTRATEGY_H_
#define _VISITSTRATEGY_H_
#include <string>
#include <memory>
#include <iostream>
#include "Player.h"
#include "Square.h"
#include "Board.h"

// Abstract base class for all strategies
class VisitStrategy {
  public:
    // What will happen if the player lands on a square corresponding to this strategy?
    virtual void acceptVisitor(std::shared_ptr<Player> player,
            std::shared_ptr<Board> board,
            std::istream& in, std::ostream& out) = 0;
};

#endif

