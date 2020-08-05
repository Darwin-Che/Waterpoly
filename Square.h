#ifndef _SQUARE_H_
#define _SQUARE_H_

#include <string>
#include <memory>
class Player;

class Square {
    std::string name;
    int location;

  public:
    Square(const std::string& name, int location);
    virtual ~Square() = default;
    void acceptVisitor(Player& player) = 0;
    std::string getName() const;
    int getLocation() const;
};

#endif

