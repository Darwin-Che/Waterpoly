#include "GooseNestingStrategy.h"

void GooseNestingStrategy::acceptVisitor(std::shared_ptr<Player> player,
        std::shared_ptr<Board> board, std::istream in, std::ostream out) {
    out << "You stepped on Goose Nesting square. You are attacked by a flock of "
        << "nesting geese, but otherwise nothing happened." << std::endl;
}

