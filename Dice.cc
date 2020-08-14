#include "Dice.h"
#include <cstdlib>
#include <iostream>
#include <vector>

bool Dice::canRoll = true;
int Dice::numDoubles = 0;
int Dice::storage = 0;
bool Dice::testing_mode = false;
std::vector<std::pair<int, int>> Dice::history;


std::pair<int, int> Dice::roll()
{
    int a = 0, b = 0;
    if (Dice::storage > 0) {
        std::pair<int, int> ans = history.back();
        std::cout << "Move by your last roll, which are " << ans.first << " and " << ans.second << std::endl;
        return ans;
    }
    if (Dice::testing_mode)
    {
        std::cout << "You are in testing mode. Please enter two dice results: " << std::endl;
        std::cin >> a >> b;
    }
    else
    {
        a = rand() % 6 + 1;
        b = rand() % 6 + 1;
    }
    std::pair<int, int> ans = std::make_pair(a, b);
    Dice::history.emplace_back(ans);
    if (a == b)
        Dice::numDoubles++;
    return ans;
}

void Dice::clear()
{
    Dice::history.clear();
    Dice::canRoll = true;
    Dice::numDoubles = 0;
    Dice::storage = 0;
}

void Dice::init(bool testing)
{
    Dice::clear();
    Dice::testing_mode = testing;
}