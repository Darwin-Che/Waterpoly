#include <vector>
#include <iostream>
#include <string>

using namespace std;

int main(){
    vector<int> vec;
    vec = vector<int>{3,4};
    for (auto i:vec){
        cout << i << " ";
    }
    cout << endl;
    string str="";
    str.insert(0,5,'1');
    cout << str<< endl;
}
