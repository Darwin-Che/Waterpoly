#ifndef TEXTVIEW_H
#define TEXTVIEW_H
#include "view.h"

class TextView: public View{
    int gridH = 4, gridW = 8;
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
        std::string getLine(const int & i);
        int getHeight();
    };
  public:
    virtual void drawBoard();
};

#endif
