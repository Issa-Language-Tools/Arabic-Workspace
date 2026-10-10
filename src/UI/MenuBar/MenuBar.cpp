#include "MenuBar.hpp"

MenuBar::MenuBar(int x, int y, int w, int h, const char* L) : Fl_Box(x, y, w, h, L) {
  box(FL_FLAT_BOX);
  color(fl_rgb_color(255, 55, 55), fl_rgb_color(200, 200, 200));

}
