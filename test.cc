#include <vector>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

class Block{
    int blockH, blockW;
    int improvement;
    int playerNum;
    std::vector<std::string> content;
    void setName(std::string & name,int blockLine);
    public:
    Block(int h, int w);
    void setContent(const int & imp,std::string & name);
    void setPlayer(const char symbol);
    std::string getLine(const int & i);
    int getHeight();
};

Block::Block(int h, int w): blockH(h), blockW(w) {
    for (int i=0; i<h+1; ++i ){
        string str="";
        str.insert(0,w+1,' ');
        content.emplace_back(str);
    }  
    playerNum = 0;
}

int min(int a, int b){
    if(a<b){
        return a;
    }
    return b;
}

void Block::setName(std::string & name, int blockLine){
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

void Block::setContent(const int & imp,std::string & name){
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

void Block::setPlayer(const char symbol){
    content[blockH-1][playerNum] = symbol;
    playerNum++;
}

std::string Block::getLine(const int & i){
    return content[i];
}

int Block::getHeight(){
    return blockH+1;
}

int main(){
    Block b {4,8};
    string str = " hell oooo";
    b.setContent(4,str);
    b.setPlayer('A');
    b.setPlayer('B');
    b.setPlayer('a');
    cout << "\033[4m"<<b.getHeight() << "\033[0m" << endl;
    for (int i=0; i<b.getHeight();i++){
        cout << b.getLine(i) << endl;
    }
}
