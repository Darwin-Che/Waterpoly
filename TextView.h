#ifndef TEXTVIEW_H
#define TEXTVIEW_H
#include "view.h"

class TextView: public View{
    int gridSpace = 8;
    class Block{
        int blockH, blockW;
        int improvement;
        std::vector<std::string> content;
        void setName(std::string & name);
      public:
        Block(int h, int w);
        void setContent(const int & imp,std::string & name);
        std::string getLine(const int & i);
    };
  public:
    virtual void drawBoard();
};

#endif
