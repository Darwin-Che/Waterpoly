#ifndef _INFO_H
#define _INFO_H
#include <string>
#include <vector>

// State of the cell i.e. either dead or alive.
enum class InfoType { player, academic};

// Information for the cell to return i.e. its state (Alive/Dead) and position in grid.
struct Info {
    InfoType type;
    std::vector<bool> boolVals;
    std::vector<int> intVals;
    std::vector<char> charVals;
    std::vector<std::string> Vals;
};

#endif
