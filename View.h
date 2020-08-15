#ifndef VIEW_H
#define VIEW_H
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include "Observer.h"

class View: public Observer{
  protected:
    //board size
    int height, width;
    //actual size of the board, add 1 to height and width
    int actualH, actualW;
    // stores the game's current state
    bool initialized;
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

    // move a player to a new location
    virtual void movePlayer(char player, int newlocation) = 0;
    // remove a player from the board
    virtual void removePlayer(char player) = 0;
    // change an acdemic building
    virtual void changeAcBuilding(int location, int newimprovement, char owner) = 0;
    // change the owner of a building
    virtual void changeBuilding(int location, char owner) = 0;
  public:
    //constructor
    View(int height, int width); 
    // Type and improvement level of the building
    enum Type { nonBuilding=0, nonAcademic, academic, lv1, lv2, lv3, lv4, lv5, maxLv=lv5 };
    // add a player
    void addPlayer(const char & ID, const int & position);
    // add a square 
    virtual void addSquare(const std::string & name, int improvement);
    // When notified by a Subject
    virtual void notify( Subject & whoNotified );
    // draw the view
    virtual void drawBoard() = 0;
};

#endif
