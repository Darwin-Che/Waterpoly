#include "TextView.h"
#include <sstream>
#include <iostream>
#include <algorithm>
using namespace std;

// Block constructor
TextView::Block::Block(int h, int w): blockH(h), blockW(w) {
    for (int i=0; i<h+1; ++i ){
        string str="";
        str.insert(0,w+1,' ');
        content.emplace_back(str);
    }  
    playerNum=0;
}

// helper function: minimum
int TextView::Block::min(int a, int b){
    if(a<b){ return a; }
    return b;
}

// set the name of a block
void TextView::Block::setName(const std::string & name, int blockLine){
    istringstream strm{name};
    string word;
    int lineIndex=0;
    while(strm >> word && blockLine < blockH-1){
        if (word.length()+lineIndex > blockW){
            if(lineIndex == 0){
                for (int i=0; i<blockW; ++i){
                    content[blockLine][i] = word[i];
                }
                blockLine++;
            }
            else{
                blockLine++;
                if (blockLine < blockH-1){
                    for (int i=0; i<min(blockW,word.length()); ++i){
                        content[blockLine][i] = word[i];
                    }
                    if(word.length() <= blockW){
                        lineIndex = word.length()+1;
                    }
                    else{
                        lineIndex = 0; 
                        blockLine++;
                    }
                }
            }
        }
        else{
            for (int i=0; i<word.length(); ++i){
                content[blockLine][i+lineIndex] = word[i];
            }
            lineIndex = lineIndex + word.length()+1;
        }
    }
}

// draw a line at the bottom of the block
void TextView::Block::setUnderLine(){
    for(int i=0; i<=blockW ; i++){
        content[blockH][i]='_';
    }
}

// draw a line in the rightmost column of the block
void TextView::Block::setRightLine(){
    for(int i=0; i<=blockH ; i++){
        content[i][blockW]='|';
    }
}

// fill in Square information 
void TextView::Block::setContent(const int & imp,const std::string & name){
    setUnderLine();
    setRightLine();
    if(imp >= academic){
        for(int i=academic; i<imp ; i++){
            // draw the improvement levels
            content[0][i-academic]='I';
        }
        for(int i=0; i<blockW ; i++){
            content[1][i]='-';
        }
        setName(name,2);
    }
    else{
        setName(name,0);
    }
}

// add a player's symbol to the block
void TextView::Block::addPlayer(const char & symbol){
    playerSymbols.push_back(symbol);
    content[blockH-1][playerNum] = symbol;
    playerNum++;
}

// remove a player from the board
void TextView::Block::removePlayer(const char & symbol){
    auto index = find(playerSymbols.begin(), playerSymbols.end(), symbol);
    if (index != playerSymbols.end()){ // if the symbol is found in this block
        playerSymbols.erase(index);
        for (int i=0; i<blockW; i++){
            content[blockH-1][i] = ' ';
        }
        playerNum--;
        for (int i=0; i<playerNum; i++){
            content[blockH-1][i] = playerSymbols[i];
        }
    }
    
}

// change the number of improvements
void TextView::Block::changeImprovement(int newimprovement){
    for(int i=0; i<blockW ; i++){
        content[0][i]=' ';
    }
    if (newimprovement >= academic){
        for(int i=academic; i<newimprovement ; i++){
            content[0][i-academic]='I';
        }
    }
}

// get the i-th line of the block
std::string TextView::Block::getLine(const int & i){
    return content[i];
}

// get the height of the block
int TextView::Block::getHeight(){
    return blockH;
}

// get the width of the block
int TextView::Block::getWidth(){
    return blockW;
}

// TextView constructor
TextView::TextView(int height, int width):View(height,width){
    gridH = 4;
    gridW = 8;
    // initialize the 2d Block vector
    for (int i=0; i<actualH; i++){
        vector<shared_ptr<Block> > row;
        for (int j=0; j<actualW; j++){
            row.push_back(make_shared<Block>(gridH,gridW));
        }
        Blocks.push_back(row);
    }
    for (int i=1 ; i< actualW-1; i++){
        Blocks[actualH-2][i]->setUnderLine();
    }
    for (int i=1 ; i< actualH-1; i++){
        Blocks[i][actualW-2]->setRightLine();
    }
}

// add a player
void TextView::addPlayer(const char & ID, const int & position){
    vector<int> pos = get2Dlocation(position);
    View::addPlayer(ID, position);
    Blocks[pos[0]][pos[1]]->addPlayer(ID);
}
// add a square 
void TextView::addSquare(const std::string & name, int improvement){
    int i = squareName.size();
    View::addSquare(name, improvement);
    vector<int> position = get2Dlocation(i);
    Blocks[position[0]][position[1]]->setContent(improvement,name);
}

// move a player to a new location
void TextView::movePlayer(char player, int newlocation){
    int oldposition = players[player];
    vector<int> old2D = get2Dlocation(oldposition);
    vector<int> new2D = get2Dlocation(newlocation);
    Blocks[old2D[0]][old2D[1]]->removePlayer(player);
    Blocks[new2D[0]][new2D[1]]->addPlayer(player);
    players[player]=newlocation;
    
    // if the board finished initializing
    if (initialized){
        // redraw the board
        this->drawBoard();
    }
}

// remove a player from the board
void TextView::removePlayer(char player){
    int position = players[player];
    vector<int> loc2D = get2Dlocation(position);
    Blocks[loc2D[0]][loc2D[1]]->removePlayer(player);
    players.erase( player );

    // if the board finished initializing
    if (initialized){
        // redraw the board
        this->drawBoard();
    }
}

// change the improvement level of a block
void TextView::changeAcBuilding(int location, int newimprovement, char owner) {
    vector<int> loc2D = get2Dlocation(location);
    Blocks[loc2D[0]][loc2D[1]]->changeImprovement(academic+newimprovement);
    improvements[location] = academic+newimprovement;

    // if the board finished initializing
    if (initialized){
        // redraw the board
        this->drawBoard();
    }
}

// change the owner of a block
void TextView::changeBuilding(int location, char owner){}

// draw the view
void TextView::drawBoard(){
    initialized = true;
    
    // draw an overline
    for (int i=0; i<=actualW*(gridW+1); i++){
        cout << "_";
    }
    cout << endl;
    for (int i=0; i<actualH; i++){
        for (int j=0; j<=gridH; j++){
            cout << "|";
            for (int k=0;k<actualW; k++){
                cout << Blocks[i][k]->getLine(j);
            }
            cout << endl;
        }
    }
}
