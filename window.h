#ifndef __WINDOW_H__
#define __WINDOW_H__
#include <X11/Xlib.h>
#include <iostream>
#include <string>

class Xwindow {
  Display *d;
  Window w;
  int s;
  GC gc;
  unsigned long colours[20];
  int width, height;

 public:
  Xwindow(int width=700, int height=700);  // Constructor; displays the window.
  ~Xwindow();                              // Destructor; destroys the window.
  Xwindow(const Xwindow&) = delete;
  Xwindow &operator=(const Xwindow&) = delete;

  enum {White=0, Black, lightGray, Green, PowderBlue, Aqua, Orange}; // Available colours.

  // show the Xwindow
  void mapXWindow();

  // fills a rectangle
  void fillRectangle(int x, int y, int width, int height, int colour=Black);

  // Draws a rectangle
  void drawRectangle(int x, int y, int width, int height, int colour=Black);

  // Draws a line
  void drawLine(int x1, int y1, int x2, int y2, int colour = Black);

  // Draws a string
  void drawString(int x, int y, std::string msg);

};

#endif

