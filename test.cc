#include <iostream>
#include "window.h"

using namespace std;

int main() {
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

