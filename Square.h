#ifndef _SQUARE_H_
#define _SQUARE_H_
#include <string>

class Square {
    std::string name;
    int location;

  public:
    Square(const std::string& name, int location);
    virtual ~Square() = default;
    std::string getName() const;
    int getLocation() const;
};

#endif

