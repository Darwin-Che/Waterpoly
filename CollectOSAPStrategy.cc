#include "CollectOSAPStrategy.h"
#include "MoneyStrategy.h"

// The player will collect $200 OSAP fund
void CollectOSAPStrategy::acceptVisitor(std::shared_ptr<Player> player,
        std::shared_ptr<Board> board, std::istream& in, std::ostream& out) {
    out << "You stepped on or over Collect OSAP square. "
        << "You will receive $200." << std::endl;

    // The work will be done by MoneyStrategy to avoid repeated code
    MoneyStrategy strat(200);
    strat.acceptVisitor(player, board, in, out);
}

