#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <memory>
#include "TextView.h"
using namespace std;

int main(){
    TextView tv(10,10);
    int i;
    /*while(cin >> i){
        vector<int> p=tv.get2Dlocation(i);
        cout << p[0]<<","<<p[1]<<endl;
    }*/
    tv.addPlayer('A');
    for (int i=0;i<40;i++){
        tv.addSquare("hello world");
    }
    tv.drawBoard();
    return 0;
}



