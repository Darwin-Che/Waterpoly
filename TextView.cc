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
void TextView::Block::setName(std::string & name, int blockLine){
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
void TextView::Block::setContent(const int & imp,std::string & name){
    setUnderLine();
    setRightLine();
    if(imp >= academic){
        for(int i=academic; i<imp ; i++){
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

// add a player's symbol to the block
void TextView::Block::removePlayer(const char & symbol){
    cout << "removing "<< symbol<<endl;
    auto index = find(playerSymbols.begin(), playerSymbols.end(), symbol);
    if (index != playerSymbols.end()){
        cout << "old players: ";
        for (int i=0; i<playerSymbols.size(); i++){
            cout<< playerSymbols[i]<<" ";
        }
        playerSymbols.erase(index);
        cout << ",new players: ";
        for (int i=0; i<playerSymbols.size(); i++){
            cout<< playerSymbols[i]<<" ";
        }
        cout << endl;
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

void TextView::movePlayer(char player, int newlocation){
    int oldposition = players[player];
    cout << player <<" from "<<oldposition << " to "<< newlocation<<endl;
    vector<int> old2D = get2Dlocation(oldposition);
    vector<int> new2D = get2Dlocation(newlocation);
    Blocks[old2D[0]][old2D[1]]->removePlayer(player);
    Blocks[new2D[0]][new2D[1]]->addPlayer(player);
    players[player]=newlocation;
}

void TextView::removePlayer(char player){
    int position = players[player];
    cout << player <<" remove from "<<position <<endl;
    vector<int> loc2D = get2Dlocation(position);
    Blocks[loc2D[0]][loc2D[1]]->removePlayer(player);
    players.erase( player );
}

void TextView::changeImprovement(int location, int newimprovement) {
    vector<int> loc2D = get2Dlocation(location);
    Blocks[loc2D[0]][loc2D[1]]->changeImprovement(academic+newimprovement);
    improvements[location] = academic+newimprovement;
}

// update the blocks
void TextView::initializeBlocks(){
    for (int i=0; i<2*(width+height); i++){ // update the squares
        vector<int> position = get2Dlocation(i);
        Blocks[position[0]][position[1]]->setContent(improvements[i],squareName[i]);
    }
    for (auto it:players){ // update the players
        vector<int> position = get2Dlocation(it.second);
        Blocks[position[0]][position[1]]->addPlayer(it.first);
    }
}

// draw the view
void TextView::drawBoard(){
    if (!initialized){
        // if the first time to draw a boardx
        initializeBlocks();
        initialized = true;
    }

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
