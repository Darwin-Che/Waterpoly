#include "CoopFeeStrategy.h"
#include "MoneyStrategy.h"

void CoopFeeStrategy::acceptVisitor(std::shared_ptr<Player> player,
        std::shared_ptr<Board> board, std::istream in, std::ostream out) {
    out << "You stepped on Coop Fee square. You will pay $150." << std::endl;
    MoneyStrategy strat(-150);
    strat.acceptVisitor(player, board, in, out);
}

