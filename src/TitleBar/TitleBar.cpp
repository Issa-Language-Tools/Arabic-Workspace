#include "TitleBar.hpp"
#include <FL/Fl_Box.H>

TitleBar::TitleBar(int x, int y, int w, int h, const char* L) : Fl_Box(x, y, w, h, L) {
  box(FL_FLAT_BOX);
  color(fl_rgb_color(55, 55, 55), fl_rgb_color(200, 200, 200));
  labelsize(12);
  labelcolor(fl_rgb_color(160, 160, 160));
}
