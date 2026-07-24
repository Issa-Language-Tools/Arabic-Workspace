#include "Panel.hpp"
#include <FL/Enumerations.H>

Panel::Panel(int x, int y, int w, int h) : Fl_Group(x, y, w, h) {
  // Styling
  box(FL_FLAT_BOX);
  color(fl_rgb_color(48, 48, 48));
}
