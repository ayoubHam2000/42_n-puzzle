#ifndef Drawer_HPP
#define Drawer_HPP

#include "Sdl_class.hpp"

class Drawer
{

  Sdl_class &sdl;
  int       color;

private:
Drawer();

public:
  Drawer(Sdl_class &sdl_class): sdl(sdl_class), color(0xffffff) {

  }

public:


  void  rectangle_hollow(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int stroke, int borders) {
    int top =     borders & 0b1000;
    int right =   borders & 0b0100;
    int bottom =  borders & 0b0010;
    int left =    borders & 0b0001;

    for (unsigned int j = 0; j < h; j++) {
      for (unsigned int i = 0; i < stroke; i++) {
        if (left)
          sdl.ft_put_pixel(x + i, y + j, this->color);
        if (right)
          sdl.ft_put_pixel(x + w - stroke + i, y + j, this->color);
      }
    }
    for (unsigned int j = 0; j < stroke; j++) {
      for (unsigned int i = 0; i < w; i++) {
        if (top)
          sdl.ft_put_pixel(x + i, y + j, this->color);
        if (bottom)
          sdl.ft_put_pixel(x + i, y + h - stroke + j, this->color);
      }
    }
  }

  void  rectangle(unsigned int x, unsigned y, unsigned int w, unsigned int h) {
    for (unsigned int j = 0; j < h; j++) {
      for (unsigned int i = 0; i < w; i++) {
        sdl.ft_put_pixel(x + i, y + j, this->color);
      }
    }
  }

  void set_color(int new_color) {
    this->color = new_color;
  }

};

#endif
