#include "CoopFeeStrategy.h"
#include "MoneyStrategy.h"

// The player will pay $150 coop fee
void CoopFeeStrategy::acceptVisitor(std::shared_ptr<Player> player,
        std::shared_ptr<Board> board, std::istream& in, std::ostream& out) {
    out << "You stepped on Coop Fee square. You will pay $150." << std::endl;

    // The work will be done by MoneyStrategy to avoid duplicate code
    MoneyStrategy strat(-150);
    strat.acceptVisitor(player, board, in, out);
}

