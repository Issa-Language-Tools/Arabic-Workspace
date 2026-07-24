#pragma once
#include <FL/Fl_Group.H>

class Panel : public Fl_Group {
public:
  Panel() : Panel(0, 0, 300, 720) {}; 
  Panel(int x, int y, int w, int h);
};
