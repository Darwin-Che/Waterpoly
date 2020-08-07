#include "Square.h"

Square::Square(const std::string& name, int location, const std::string& description)
    : name(name), location(location), description(description) {}

std::string Square::getName() const {
    return name;
}

int Square::getLocation() const {
    return location;
}

std::string Square::getInfo() const {
    return description;
}
