#ifndef _SQUARE_H_
#define _SQUARE_H_
#include <string>

class Square {
    std::string name;
    int location;
    std::string description;
  public:
    Square(const std::string& name, int location, const std::string& description);
    virtual ~Square() = default;
    std::string getName() const;
    int getLocation() const;
    virtual std::string getInfo() const;
};

#endif

