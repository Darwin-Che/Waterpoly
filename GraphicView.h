#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include "View.h"
#include "window.h"
#include <memory>

class GraphicsView: public View{
    // window size
    int windowSize;
    // maps each player to a color
    std::map<char,int> playerColors;
    // the grahic window
    Xwindow win;
    // stores the information of each square on the board
    class Block{
      public:
        // position of the block
        int posX, posY;
        // dimensions of the block
        int blockH, blockW;
        // owner of the block
        char owner;
        // the char vector that stores each player on this block
        std::string playerSymbols;
        // the string vector that stores each block's name
        std::vector<std::string> nameString;
        // improvement leve
        int improvement;
        // constructor
        Block(int x, int y, int h, int w, int improvement, std::string name);
    };
    // a vector of blocks
    std::vector<std::shared_ptr<Block>> blocks;
    // move a player to a new location
    virtual void movePlayer(char player, int newlocation);
    // remove a player from the board
    virtual void removePlayer(char player);
    // change the improvement level of a block
    virtual void changeImprovement(int location, int newimprovement);
    // change an acdemic building
    virtual void changeAcBuilding(int location, int newimprovement, char owner);
    // change the owner of a building
    virtual void changeBuilding(int location, char owner);
    // draw the Block b
    void drawBlock(std::shared_ptr<Block> b);
 
  public:
    // constructor
    GraphicsView(int height, int width);
    // draw the view
    virtual void drawBoard();
    // add a player
    virtual void addPlayer(const char & ID, const int & position);
    // add a square 
    virtual void addSquare(const std::string & name, int improvement);
};

#endif

