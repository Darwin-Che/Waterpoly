#include "GraphicView.h"
#include <sstream>
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

// Block constructor
GraphicsView::Block::Block(int x, int y, int h, int w, int improvement, std::string name):
posX(x), posY(y), blockH(h), blockW(w), improvement(improvement) {
    playerSymbols = "";
    istringstream strm{name};
    string word;
    vector<string> parse;
    owner = ' ';

    // check each word of the name of the square, and trim them to fit the display
    while (strm>>word){
        if (word.size() * 6 > blockW){
            parse.push_back( word.substr(0, blockW/5) );
        }
        else{
            parse.push_back( word );
        }
        
    }
    string namepart="";
    // arrange each word of the name of the square into lines
    for (auto it: parse){
        if ( (namepart.size() +it.size()) * 6 > blockW ){
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
    playerColors[' '] = Xwindow::White;
}

// move a player to a new location
void GraphicsView::movePlayer(char player, int newlocation){
    int oldposition = players[player];
    // remove the player's symbol of its old position
    string * oldPos = & blocks[oldposition]->playerSymbols;
    oldPos->erase(remove (oldPos->begin(), oldPos->end(), player), oldPos->end());
    // add the player's symbol to its new position
    blocks[newlocation]->playerSymbols += player;

    players[player]=newlocation;
    // if the board finished initializing
    if (initialized){
        // redraw the two blocks
        drawBlock(blocks[newlocation]);
        drawBlock(blocks[oldposition]);
    }
}

// remove a player from the board
void GraphicsView::removePlayer(char player){
    int oldposition = players[player];
    // remove the player's symbol of its current position
    string * oldPos = & blocks[oldposition]->playerSymbols;
    oldPos->erase(remove (oldPos->begin(), oldPos->end(), player), oldPos->end());
    // remove the player from the player list
    players.erase(player);
     // if the board finished initializing
    if (initialized){
        // redraw the block
        drawBlock(blocks[oldposition]);
    }
}

// change the improvement level of a block
void GraphicsView::changeImprovement(int location, int newimprovement){
    blocks[location]->improvement = academic+newimprovement;
    improvements[location] = academic+newimprovement;

    if (initialized){
        // redraw the block
        drawBlock(blocks[location]);
    }
}

// change an acdemic building
void GraphicsView::changeAcBuilding(int location, int newimprovement, char owner){
    if (owner != blocks[location]->owner ){
        changeBuilding(location, owner);
    }
    else{
        changeImprovement(location, newimprovement);
    }
}

// change the owner of a block
void GraphicsView::changeBuilding(int location, char owner){
    blocks[location]->owner = owner;
    if (initialized){
        // redraw the block
        drawBlock(blocks[location]);
    }
}

// draw the Block b
void GraphicsView::drawBlock(shared_ptr<Block> b){
    int x = b->posX;
    int y = b->posY;
    int line = 12;
    if (b->improvement >= View::academic){
        win.fillRectangle(x,y,b->blockW,b->blockH,playerColors[b->owner]);
        // draw the improvement level
        string imp (b->improvement - academic, 'I');
        win.drawString(x+2, y+12, imp);
        win.drawLine(x, y+13, x+b->blockW, y+13, Xwindow::Indigo);
        line = 24;
    }
    else if(b->improvement == View::nonAcademic){
        win.fillRectangle(x,y,b->blockW,b->blockH,playerColors[b->owner]);
    }
    else{
        win.fillRectangle(x,y,b->blockW,b->blockH,Xwindow::BurlyWood);
    }
    // draw the name of the square
    for (int i=0 ;i<b->nameString.size(); i++){
        if (line > y+b->blockH-14){
            break;
        }
        win.drawString(x+2, y+line, b->nameString[i]);
        line += 12;
    }
    
    for (int i=0; i<b->playerSymbols.size();i++){
        win.fillRectangle(x+1+i*7,y+b->blockH-12,7,12,playerColors[b->playerSymbols[i]]);
        win.drawString(x+2+i*7,y+b->blockH-2,string(1,b->playerSymbols[i]));
    }
    // draw the player symbols on this square
    //win.drawString(x+2, y+b->blockH-2,b->playerSymbols);
    win.drawRectangle(x,y,b->blockW,b->blockH,Xwindow::Black);
}

// initialize the blocks
void GraphicsView::initializeBlocks(){
    // initialize the blocks using square informations
    for (int i=0; i < squareName.size(); i++){
        auto pos = get2Dlocation(i);
        int posX = 700*pos[1]/actualW;
        int posY = 700*pos[0]/actualH;
        int w = 700*(pos[1]+1)/actualW - posX;
        int h = 700*(pos[0]+1)/actualH - posY;
        auto block = make_shared<Block>(posX,posY,h,w,improvements[i],squareName[i]);
        blocks.push_back(block);
    }
    // add the players to their block
    int i=0;
    for (auto p: players){
        blocks[p.second]->playerSymbols += p.first;
        playerColors[p.first] = i+Xwindow::pColor;
        i++;
    }
}

// draw the view
void GraphicsView::drawBoard(){
    if (!initialized){
        // if the first time to draw a boardx
        initializeBlocks();
        win.mapXWindow();
        initialized = true;
    }
    for (auto b : blocks){
        drawBlock(b);
    }
}
