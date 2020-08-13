#include <iostream>
#include <sstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
//#include "window.h"
/*
using namespace std;

int main() {
  int blockW = 62;
  string name = "Hello 12345676543212345 helo h h h";
  istringstream strm{name};
  string word;
  vector<string> parse;
  vector<string> nameString;
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
      if ( (namepart.size() +it.size()) * 5 > blockW ){
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
  for (auto it:nameString){
    cout << it << endl;
  }
  int d = 700;
  Xwindow w(d,d);

  for (int i=0; i<11; i++){
    w.drawRectangle(0, d*i/11, d/11,d*(i+1)/11-d*i/11, Xwindow::Black);
  }

  w.drawString(50, 50, "Hello!");

  w.drawRectangle(0,0,10,10,Xwindow::Black);
  w.drawString(2, 24, "1234567891");
  w.drawString(2, 12, "III");

  w.drawString(50, 100, "ABCD");

  w.drawString(50, 150, "Hello!");

  //Xwindow w2(199, 199);
  //w2.drawString(50, 100, "ABCD");


  //w.showAvailableFonts();

  char c;
  cin >> c;
}
*/

