#include "Square.h"

Square::Square(std::string& name, int location)
    : name(name), location(location) {}

std::string Square::getName() const {
    return name;
}

int Square::getLocation() const {
    return location;
}

