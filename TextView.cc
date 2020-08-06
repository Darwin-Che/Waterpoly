#include "TextView.h"
#include <sstream>
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

void TextView::Block::setContent(const int & imp,std::string & name){
    for(int i=0; i<blockW ; i++){
        content[blockH][i]='_';
    }
    for(int i=0; i<=blockH ; i++){
            content[i][blockW]='|';
        }
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
    return blockH+1;
}

void TextView::drawBoard(){

}
