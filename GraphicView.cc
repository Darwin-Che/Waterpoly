#include "GraphicView.h"
#include <sstream>
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

GraphicsView::Block::Block(int x, int y, int h, int w, int improvement, std::string name):
posX(x), posY(y), blockH(h), blockW(w), improvement(improvement) {
    playerSymbols = "";
    istringstream strm{name};
    string word;
    vector<string> parse;
    while (strm>>word){
        if (word.size() * 5 > blockW){
            parse.push_back( word.substr(0, blockW/5) );
        }
        else{
            parse.push_back( word );
        }
        
    }
    string namepart="";
    for (auto it: parse){
        if ( (namepart.size() +it.size()+1) * 5 > blockW ){
            nameString.push_back(namepart);
            namepart = it;
        }
        else{
            namepart += it+" ";
        }
    }
    if (namepart.size() > 0){
        nameString.push_back(namepart);
    }
} 

// constructor
GraphicsView::GraphicsView(int height, int width): View(height, width){
}

void GraphicsView::movePlayer(char player, int newlocation){
    int oldposition = players[player];

    string * oldPos = & blocks[oldposition]->playerSymbols;
    oldPos->erase(remove (oldPos->begin(), oldPos->end(), player), oldPos->end());
    blocks[newlocation]->playerSymbols += player;

    players[player]=newlocation;
    
    if (initialized){
        drawBlock(blocks[newlocation]);
        drawBlock(blocks[oldposition]);
    }
}

void GraphicsView::removePlayer(char player){
    int oldposition = players[player];

    string * oldPos = & blocks[oldposition]->playerSymbols;
    oldPos->erase(remove (oldPos->begin(), oldPos->end(), player), oldPos->end());

    players.erase(player);
    
    if (initialized){
        drawBlock(blocks[oldposition]);
    }
}

void GraphicsView::changeImprovement(int location, int newimprovement){
    blocks[location]->improvement = academic+newimprovement;
    improvements[location] = academic+newimprovement;

    if (initialized){
        drawBlock(blocks[location]);
    }
}

// draw the Block b
void GraphicsView::drawBlock(shared_ptr<Block> b){
    int x = b->posX;
    int y = b->posY;
    int line = 12;
    if (b->improvement >= View::academic){
        win.fillRectangle(x,y,b->blockW,b->blockH,Xwindow::White);
        string imp (b->improvement - academic, 'I');
        win.drawString(x+2, y+12, imp);
        win.drawLine(x, y+13, x+b->blockW, y+13, Xwindow::Orange);
        line = 24;
    }
    else if(b->improvement == View::nonAcademic){
        win.fillRectangle(x,y,b->blockW,b->blockH,Xwindow::White);
    }
    else{
        win.fillRectangle(x,y,b->blockW,b->blockH,Xwindow::lightGray);
    }
    for (int i=0 ;i<b->nameString.size(); i++){
        if (line > y+b->blockH-14){
            break;
        }
        win.drawString(x+2, y+line, b->nameString[i]);
        line += 12;
    }
    
    win.drawString(x+2, y+b->blockH-2,b->playerSymbols);
    win.drawRectangle(x,y,b->blockW,b->blockH,Xwindow::Black);
}

// initialize the blocks
void GraphicsView::initializeBlocks(){
    for (int i=0; i < squareName.size(); i++){
        auto pos = get2Dlocation(i);
        int posX = 700*pos[1]/actualW;
        int posY = 700*pos[0]/actualH;
        int w = 700*(pos[1]+1)/actualW - posX;
        int h = 700*(pos[0]+1)/actualH - posY;
        auto block = make_shared<Block>(posX,posY,h,w,improvements[i],squareName[i]);
        blocks.push_back(block);
    }
    for (auto p: players){
        blocks[p.second]->playerSymbols += p.first;
    }
}

// draw the view
void GraphicsView::drawBoard(){
    if (!initialized){
        // if the first time to draw a boardx
        initializeBlocks();
        initialized = true;
    }
    win.mapXWindow();
    for (auto b : blocks){
        drawBlock(b);
    }
}
