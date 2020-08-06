#ifndef TEXTVIEW_H
#define TEXTVIEW_H
#include "view.h"
#include <memory>

class TextView: public View{
    int gridH = 4, gridW = 8;

    // the string representation of each square
    class Block{
        int blockH, blockW;
        int improvement;
        int playerNum;
        std::vector<std::string> content;
        int min(int x, int y);
        void setName(std::string & name, int blockLine);
      public:
        Block(int h, int w);
        void setContent(const int & imp,std::string & name);
        void setPlayer(const char & symbol);
        void setUnderLine();
        void setRightLine();
        std::string getLine(const int & i);
        int getHeight();
        int getWidth();
    };

    std::vector<std::vector<std::unique_ptr<Block>>> Blocks;
    void updateBlocks();
  public:
    TextView(int height, int widths);
    virtual void drawBoard();
};

#endif
