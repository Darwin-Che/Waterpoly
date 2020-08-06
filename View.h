#ifndef VIEW_H
#define VIEW_H
#include <vector>
#include <map>
#include <string>
#include "Observer.h"

class View: public Observer{
  protected:
    //board size
    int height, width;
    //actual size of the board, add 1 to height and width
    int actualH, actualW;
    // maps each player's ID to the player's current position
    std::map<char,int> players;
    // the string vector that stores each square's name
    // the squares start with the beginning square and the rest are in clockwise order
    std::vector<std::string> squareName;
    // the int vector that stores each square's improvements
    // -1 stand for non-academic buildings
    std::vector<int> improvements;
    // take a 1-d location and calculate its 2-d position on the board
    std::vector<int> get2Dlocation(int location);
    
  public:
    //constructor
    View(int height, int width); 
    // add a player
    void addPlayer(const char & ID);
    // add a square 
    void addSquare(const std::string & name);
    // When notified by a Subject
    virtual void notify( Subject & whoNotified );
    // draw the view
    virtual void drawBoard() = 0;
};

#endif