#ifndef _SQUARE_H_
#define _SQUARE_H_

#include <string>
#include <memory>
class VisitStrategy;
class Player;

class Square {
    std::string name;
    int location;
    std::shared_ptr<VisitStrategy> visitStrategy;

  public:
    Square(const std::string& name, int location,
            std::shared_ptr<VisitStrategy> visitStrategy);
    virtual ~Square() = default;
    void acceptVisitor(Player& player);
    std::string getName() const;
    int getLocation() const;
};

#endif

