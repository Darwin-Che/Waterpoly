#include "TextView.h"
using namespace std;

void TextView::Block::Block(int h, int w): blockH(h), blockW(w) {
    for (int i=0; i<h+1; ++i ){
        string str="";
        str.insert(0,w+1,' ');
        content.emplace_back(str);
    }  
}

void TextView::drawBoard(){

}
