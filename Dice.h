#ifndef _DICE_H
#define _DICE_H

#include <vector>
#include <string>

class Dice
{
    static std::vector<std::pair<int, int>> history;

public:
    static int numDoubles;
    static bool canRoll;
    static bool testing_mode;
    static int storage;
    static std::pair<int, int> roll();
    static void init(bool testing);
    static void clear();
    static int cup();
};

#endif
