#include "Square.h"
#include "VisitStrategy.h"

Square::Square(std::string& name, int location,
        std::shared_ptr<VisitStrategy> visitStrategy)
    : name(name), location(location), visitStrategy(visitStrategy) {}

void Square::acceptVisitor(Player& player) {
    visitStrategy->visit(player);
}

std::string Square::getName() const {
    return name;
}

int Square::getLocation() const {
    return location;
}

