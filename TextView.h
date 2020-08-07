#ifndef TEXTVIEW_H
#define TEXTVIEW_H
#include "View.h"
#include <memory>

class TextView: public View{
    int gridH, gridW;

    // the string representation of each square
    class Block{
        // dimensions of the block
        int blockH, blockW;
        // number of improvements, -1 stand for non-academic building
        int improvement;
        // number of players in the block
        int playerNum;
        // the string vector that stores each blockline's information
        std::vector<std::string> content;
        // helper function: minimum
        int min(int x, int y);
        // set the name of a block
        void setName(std::string & name, int blockLine);
        
      public:
        // constructor
        Block(int h, int w);
        // fill in Square information 
        void setContent(const int & imp,std::string & name);
        // add a player's symbol to the block
        void setPlayer(const char & symbol);
        // get the i-th line of the block
        std::string getLine(const int & i);
        // get the height of the block
        int getHeight();
        // get the width of the block
        int getWidth();
        // draw a line at the bottom of the block
        void setUnderLine();
        // draw a line in the rightmost column of the block
        void setRightLine();
    };
    // 2d vector storing blocks
    // each block represents a square
    std::vector<std::vector<std::shared_ptr<Block> > > Blocks;
    // update the blocks
    void updateBlocks();
  public:
    // constructor
    TextView(int height, int widths);
    // draw the view
    virtual void drawBoard();
};

#endif
