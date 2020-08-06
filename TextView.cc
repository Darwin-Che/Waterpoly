#include "TextView.h"
#include <sstream>
#include <iostream>
using namespace std;

TextView::Block::Block(int h, int w): blockH(h), blockW(w) {
    for (int i=0; i<h+1; ++i ){
        string str="";
        str.insert(0,w+1,' ');
        content.emplace_back(str);
    }  
    playerNum=0;
}

int TextView::Block::min(int a, int b){
    if(a<b){ return a; }
    return b;
}

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
        else{
            for (int i=0; i<word.length(); ++i){
                content[blockLine][i+lineIndex] = word[i];
            }
            lineIndex = lineIndex + word.length()+1;
        }
    }
}

void TextView::Block::setUnderLine(){
    for(int i=0; i<blockW ; i++){
        content[blockH][i]='_';
    }
}
void TextView::Block::RightLine(){
    for(int i=0; i<=blockH ; i++){
        content[i][blockW]='|';
    }
}

void TextView::Block::setContent(const int & imp,std::string & name){
    setUnderLine();
    setRightLine();
    if(imp >= 0){
        for(int i=0; i<imp ; i++){
            content[0][i]='I';
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

void TextView::Block::setPlayer(const char & symbol){
    content[blockH-1][playerNum] = symbol;
    playerNum++;
}

std::string TextView::Block::getLine(const int & i){
    return content[i];
}

int TextView::Block::getHeight(){
    return blockH;
}
int TextView::Block::getWidth(){
    return blockW;
}

TextView::TextView(int height, int width):View(height,width){
    for (int i=0; i<height; i++){
        vector<unique_ptr<Block>> row;
        for (int j=0; j<width; j++){
            row.emplace_back(make_unique<Block>(gridH,gridW));
        }
        Blocks.emplace_back(row);
    }
}

void TextView::updateBlocks(){
    for (auto it:players){
        vector<int> position = get2Dlocation(it.second);
        Blocks[position[0]][position[1]]->setPlayer(it.first);
    }
    for (int i=0; i<2*(width+height); i++){
        vector<int> position = get2Dlocation(i);
        Blocks[position[0]][position[1]]->setContent(improvements[i],squareName[i]);
    }
    for (int i=1 ; i< width-1; i++){
        Blocks[height-2][i]->setUnderLine();
    }
    for (int i=1 ; i< height-1; i++){
        Blocks[width-2][i]->setRightLine();
    }
}

void TextView::drawBoard(){
    updateBlocks();
    for (int i=0; i<width*(gridW+1); i++){
        cout << "_";
    }
    cout << endl;
    int yAxis = 0;
    for (int i=0; i<height*(gridH+1); i++){
        y = i/(gridH+1);
        cout << "|";
        for (int j=0; j<width;j++){
            cout << Blocks[y][j]->getLine(i);
        }
        cout << endl;
    }
}
