#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <unistd.h>
#include "window.h"

using namespace std;

Xwindow::Xwindow(int width, int height):width(width), height(height) {

  d = XOpenDisplay(NULL);
  if (d == NULL) {
    cerr << "Cannot open display" << endl;
    exit(1);
  }
  s = DefaultScreen(d);
  w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, width, height, 1,
                          BlackPixel(d, s), WhitePixel(d, s));
  XSelectInput(d, w, ExposureMask | KeyPressMask);
  //XMapWindow(d, w);
}

Xwindow::~Xwindow() {
  XFreeGC(d, gc);
  XCloseDisplay(d);
}

// show the Xwindow
void Xwindow::mapXWindow(){
  XMapRaised(d, w);
  Pixmap pix = XCreatePixmap(d,w,width,
        height,DefaultDepth(d,DefaultScreen(d)));
  gc = XCreateGC(d, pix, 0,(XGCValues *)0);

  XFlush(d);
  XFlush(d);

  // Set up colours.
  XColor xcolour;
  Colormap cmap;
  char color_vals[20][20]={"white", "black", "light gray", "pale goldenrod", "Indigo", 
  "tomato", "orange", "yellow", "light sea green", "deep sky blue" ,"powder blue", "violet", "hot pink"};

  cmap=DefaultColormap(d,DefaultScreen(d));
  for(int i=0; i < 13; ++i) {
      XParseColor(d,cmap,color_vals[i],&xcolour);
      XAllocColor(d,cmap,&xcolour);
      colours[i]=xcolour.pixel;
  }

  XSetForeground(d,gc,colours[Black]);

  // Make window non-resizeable.
  XSizeHints hints;
  hints.flags = (USPosition | PSize | PMinSize | PMaxSize );
  hints.height = hints.base_height = hints.min_height = hints.max_height = height;
  hints.width = hints.base_width = hints.min_width = hints.max_width = width;
  XSetNormalHints(d, w, &hints);

  XSynchronize(d,True);

  usleep(1000);
  
}

// fills a rectangle
void Xwindow::fillRectangle(int x, int y, int width, int height, int colour) {
  XSetForeground(d, gc, colours[colour]);
  XFillRectangle(d, w, gc, x, y, width, height);
  XSetForeground(d, gc, colours[Black]);
}

// draws a rectangle
void Xwindow::drawRectangle(int x, int y, int width, int height, int colour) {
  XSetForeground(d, gc, colours[colour]);
  XDrawRectangle(d, w, gc, x, y, width, height);
  XSetForeground(d, gc, colours[Black]);
}

// draws a line
void Xwindow::drawLine(int x1, int y1, int x2, int y2, int colour) {
  XSetForeground(d, gc, colours[colour]);
  XDrawLine(d, w, gc, x1, y1, x2, y2);
  XSetForeground(d, gc, colours[Black]);
}

// draws a string
void Xwindow::drawString(int x, int y, string msg) {
  XDrawString(d, w, DefaultGC(d, s), x, y, msg.c_str(), msg.length());
}


