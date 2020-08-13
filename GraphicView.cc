#include "GraphicView.h"
#include <sstream>
#include <iostream>
#include <string>
#include <algorithm>

GraphicsView::Block::Block(int x, int y, int h, int w, int improvement, std::string name):
posX(x), posY(y), blockH(h), blockW(w), improvement(improvement) {
    istringstream strm{name};
    string word;
    vector<string> parse;
    while (strm>>word){
        parse.pushback( word );
    }
    for (auto it: parse){
        
    }
} 

void GraphicsView::movePlayer(char player, int newlocation){}

void GraphicsView::removePlayer(char player){}

void GraphicsView::changeImprovement(int location, int newimprovement){}

// constructor
GraphicsView::GraphicsView(int height, int width): View(height, width){
    windowSize = 700;
    BlockH = windowSize/actualH;
    blockW = windowSize/actualW;
    win = Xwindow(windowSize,windowSize);
}
// draw the view
void GraphicsView::drawBoard(){

}
